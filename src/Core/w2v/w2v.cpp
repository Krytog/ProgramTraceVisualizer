#include "w2v.h"
#include "Clamping.h"
#include "Core/Plotting/PlotMesh/Plot3DMesh.h"
#include "UI/Widgets/ProgressWidget.h"
#include "UmapWrapper.h"

#include <memory>
#include <string>
#include <word2vec++/include/word2vec.hpp>

#include <stdexcept>
#include <thread>
#include <fstream>

#define ERROR_MESSAGE_TRAIN_FAILED "W2V: Model failed to train: "

namespace {
static const constexpr size_t kDefaultCells = 1024;

static w2v::trainSettings_t GetW2VModelSettings() {
    w2v::trainSettings_t settings;
    settings.endOfSentenceChars = "";
    settings.wordDelimiterChars = "";
    settings.minWordFreq = 1;
    settings.withSG = true;
    settings.threads = std::thread::hardware_concurrency();
    settings.iterations = 40;
    settings.withHS = true;
    settings.maxWordLen = 8;
    return settings;
}

struct StatsPointers {
    std::atomic<float>* parsing_progress_;
    std::atomic<float>* training_progress_;
    std::atomic<size_t>* different_words_count_;
    std::atomic<size_t>* total_words_count_;
};

static void TrainModel(w2v::w2vModel_t* model, const w2v::trainSettings_t& settings,
                       const std::string& filename, const StatsPointers* stats) {
    const auto trained = model->train(
        settings, filename, "", [stats](float percent) { *(stats->parsing_progress_) = percent; },
        [stats](size_t different_words, size_t train_words, size_t total_words) {
            (void)train_words;
            *(stats->different_words_count_) =
                different_words - 1;  // -1 as there will always be technical word </s>
            *(stats->total_words_count_) = total_words;
            *(stats->parsing_progress_) = 100.0f;  // just to ensure that this stat is correct
        },
        [stats](float alpha, float percent) {
            (void)alpha;
            *(stats->training_progress_) = percent;
        });
    if (!trained) {
        std::string error_message = ERROR_MESSAGE_TRAIN_FAILED;
        error_message += model->errMsg();
        throw std::runtime_error(error_message);
    }
}

static std::vector<double> GetW2VEmbedding(w2v::w2vModel_t* model, size_t dim) {
    const auto total_objects = model->modelSize() - 1;
    std::vector<double> data(total_objects * dim);
    size_t position = 0;
    std::ofstream out("dump.embd");
    for (const auto& [word, vector] : model->map()) {
        if (word == "</s>") {
            continue;
        }
        for (size_t feature = 0; feature < dim; ++feature) {
            data[position] = vector.at(feature);
            out << vector.at(feature) << " ";
        }
        out << "\n";
        ++position;
    }
    return data;
}

static std::vector<double> GetUMAPEmbedding(const std::vector<double>& data, size_t target_dim,
                                            size_t objects_count, size_t initial_dim,
                                            const W2VHandler::Params& params) {
    std::vector<double> embedding(objects_count * target_dim);
    umap::TrainParams train_params;
    train_params.neighbours = params.neighbours;
    train_params.epochs = params.epochs;
    train_params.min_distance = params.min_dist;
    umap::TrainEmbedding(initial_dim, objects_count, data.data(), target_dim, embedding.data(), train_params);
    return embedding;
}

static size_t GetLinearIndex(size_t object_index, size_t dimension_index, size_t dimensions) {
    return dimension_index + object_index * dimensions;
}

}  // namespace

W2VHandler::W2VHandler(const std::string& filename)
    : progress_wigdet_(std::make_unique<ProgressWidget>()) {
    current_params_.dimension = 0;
    current_params_.cells = 0;
    StartPrepare(filename);
}

W2VHandler::~W2VHandler() {
    if (worker_) {
        worker_->join();
    }
}

//#include "kek.h"

void W2VHandler::InitW2VEmbedding(const std::string& filename) {
    const auto settings = GetW2VModelSettings();
    auto model = std::make_unique<w2v::w2vModel_t>();
    StatsPointers stats{.parsing_progress_ = &parsing_progress_,
                        .training_progress_ = &training_progress_,
                        .different_words_count_ = &different_words_count_,
                        .total_words_count_ = &total_words_count_};
    TrainModel(model.get(), settings, filename, &stats);
    initial_dim_ = settings.size;
    w2v_embedding_ = std::move(GetW2VEmbedding(model.get(), initial_dim_));
    //w2v_embedding_ = kData;
    //initial_dim_ = 64;
    objects_count_ = w2v_embedding_.size() / initial_dim_;
}

void W2VHandler::SetUmapEmbedding(size_t target_dim) {
    worker_->join();
    worker_ = std::move(std::make_unique<std::thread>([this, target_dim]() {
        ready_ = false;
        umap_embedding_ = std::move(
            GetUMAPEmbedding(w2v_embedding_, target_dim, objects_count_, initial_dim_, current_params_));
        is_data_loaded_ = false;
        ready_ = true;
    }));
    is_umap_embedding_recalc_pending_ = false;
}

void W2VHandler::SetPlotSize(size_t cells) {
    plot_->SetGridSize(cells);
    current_params_.cells = cells;
}

size_t W2VHandler::GetPlotSize() const {
    return current_params_.cells;
}

void W2VHandler::SetDimension(size_t dimension) {
    current_params_.dimension = dimension;
    ready_ = false;
    is_umap_embedding_recalc_pending_ = true;
}

size_t W2VHandler::GetDimension() const {
    return current_params_.dimension;
}

std::vector<double> W2VHandler::GetObjectAtIndex(size_t index) const {
    std::vector<double> output(current_params_.dimension);
    for (size_t i = 0; i < current_params_.dimension; ++i) {
        output[i] = umap_embedding_.at(GetLinearIndex(index, i, current_params_.dimension));
    }
    return output;
}

std::pair<std::vector<double>, std::vector<double>> W2VHandler::GetMinMax() const {
    std::vector<double> mins(current_params_.dimension);
    std::vector<double> maxs(current_params_.dimension);
    for (size_t i = 0; i < current_params_.dimension; ++i) {
        mins[i] = std::numeric_limits<double>::max();
        maxs[i] = std::numeric_limits<double>::min();
        for (size_t j = 0; j < objects_count_; ++j) {
            const auto current = umap_embedding_.at(GetLinearIndex(j, i, current_params_.dimension));
            if (mins[i] > current) {
                mins[i] = current;
            }
            if (maxs[i] < current) {
                maxs[i] = current;
            }
        }
    }
    return {std::move(mins), std::move(maxs)};
}

std::vector<float> W2VHandler::GetPreparedData() const {
    const auto [mins, maxs] = std::move(GetMinMax());
    Clamper clamper(mins, maxs);
    std::vector<float> output;
    output.reserve(objects_count_ * (current_params_.dimension + 1));
    for (size_t i = 0; i < objects_count_; ++i) {
        auto object = std::move(GetObjectAtIndex(i));
        clamper.ClampValue(object);
        for (const auto coord : object) {
            output.push_back(coord);
        }
        output.push_back(1.0f);  // temp no color change
    }
    return output;
}

const IRenderable* W2VHandler::GetPlot() const {
    if (!ready_) {
        return progress_wigdet_.get();
    }
    return plot_.get();
}

bool W2VHandler::IsReady() const {
    return ready_;
}

void W2VHandler::StartPrepare(const std::string& filename) {
    worker_ = std::make_unique<std::thread>([this, &filename]() {
        InitW2VEmbedding(filename);
        training_progress_ = 100.0f;
        static const constexpr size_t kDefaultDimension = 2;
    });
}

size_t W2VHandler::GetDifferentWordCount() const {
    return different_words_count_;
}

size_t W2VHandler::GetTotalWordCount() const {
    return total_words_count_;
}

float W2VHandler::GetParsingProgress() const {
    return parsing_progress_;
}

float W2VHandler::GetTrainingProgress() const {
    return training_progress_;
}

void W2VHandler::LoadData() {
    const auto data = std::move(GetPreparedData());
    plot_->LoadData(data.data(), data.size() * sizeof(GLfloat));
    is_data_loaded_ = true;
}

void W2VHandler::Update() {
    if (is_umap_embedding_recalc_pending_) {
        progress_wigdet_->GoToWaitingMode();
        SetUmapEmbedding(current_params_.dimension);
    }
    if (ready_) {
        if (!is_data_loaded_) {
            LoadData();
        }
    } else {
        progress_wigdet_->SetProgress(training_progress_);
    }
}

void W2VHandler::SetProgressParams(float width, float height) {
    if (!progress_wigdet_) {
        return;
    }
    progress_wigdet_->SetResolution(width, height);
}

void W2VHandler::StartRecalculate(const Params& params) {
    current_params_ = params;
    if (training_progress_ < 100.0f) {
        return;
    }
    SetDimension(params.dimension);
    RecreatePlot();
    plot_->SetHighColor(params.color);
}

void W2VHandler::SetColor(const float* color) {
    plot_->SetHighColor(color);
}

void W2VHandler::RecreatePlot() {
    if (current_params_.dimension == 2) {
        plot_as_plot3d_ = nullptr;
        plot_ = std::make_unique<Plot2DMesh>(current_params_.cells);
    } else {
        plot_ = std::make_unique<Plot3DMesh>(current_params_.cells);
        plot_as_plot3d_ = dynamic_cast<Plot3DMesh*>(plot_.get());
    }
}