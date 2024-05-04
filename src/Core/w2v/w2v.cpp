#include "w2v.h"
#include "Clamping.h"
#include "UmapWrapper.h"

#include <memory>
#include <string>
#include <word2vec++/include/word2vec.hpp>
#include "Core/Plotting/Plot2DMesh/Plot2DMesh.h"
#include "Graphics/Primitives/IRenderable.h"

#include <stdexcept>
#include <thread>

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
            *(stats->training_progress_) = percent * std::thread::hardware_concurrency();
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
    size_t object = 0;
    for (const auto& [word, vector] : model->map()) {
        if (word == "</s>") {
            continue;
        }
        for (size_t feature = 0; feature < dim; ++feature) {
            data[feature * total_objects + object] = vector.at(feature);
        }
        ++object;
    }
    return data;
}

static std::vector<double> GetUMAPEmbedding(const std::vector<double>& data, size_t target_dim,
                                            size_t objects_count, size_t initial_dim) {
    std::vector<double> embedding(objects_count * target_dim);
    umap::TrainEmbedding(initial_dim, objects_count, data.data(), target_dim, embedding.data());
    return embedding;
}
}  // namespace

W2VHandler::W2VHandler(const std::string& filename)
    : plot_(std::make_unique<Plot2DMesh>(kDefaultCells)), plot_size_(kDefaultCells) {
    StartPrepare(filename);
}

W2VHandler::~W2VHandler() {
    if (worker_) {
        worker_->join();
    }
}

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
    objects_count_ = w2v_embedding_.size() / initial_dim_;
}

void W2VHandler::SetUmapEmbedding(size_t target_dim) {
    if (current_umap_dim_ == target_dim) {
        return;
    }
    umap_embedding_ = std::move(GetUMAPEmbedding(w2v_embedding_, target_dim, objects_count_, initial_dim_));
    current_umap_dim_ = target_dim;
}

void W2VHandler::SetPlotSize(size_t cells) {
    plot_->SetGridSize(cells);
    plot_size_ = cells;
}

size_t W2VHandler::GetPlotSize() const {
    return plot_size_;
}

void W2VHandler::SetDimension(size_t dimension) {
    SetUmapEmbedding(dimension);
    is_data_loaded_ = false;
}

std::vector<double> W2VHandler::GetObjectAtIndex(size_t index) const {
    std::vector<double> output(current_umap_dim_);
    for (size_t i = 0; i < current_umap_dim_; ++i) {
        output[i] = umap_embedding_.at(i * objects_count_ + index);
    }
    return output;
}

std::pair<std::vector<double>, std::vector<double>> W2VHandler::GetMinMax() const {
    std::vector<double> mins(current_umap_dim_);
    std::vector<double> maxs(current_umap_dim_);
    for (size_t i = 0; i < current_umap_dim_; ++i) {
        mins[i] = std::numeric_limits<double>::max();
        maxs[i] = std::numeric_limits<double>::min();
        for (size_t j = 0; j < objects_count_; ++j) {
            if (mins[i] > umap_embedding_.at(i * objects_count_ + j)) {
                mins[i] = umap_embedding_.at(i * objects_count_ + j);
            }
            if (maxs[i] < umap_embedding_.at(i * objects_count_ + j)) {
                maxs[i] = umap_embedding_.at(i * objects_count_ + j);
            }
        }
    }
    return {std::move(mins), std::move(maxs)};
}

std::vector<float> W2VHandler::GetPreparedData() const {
    const auto [mins, maxs] = std::move(GetMinMax());
    Clamper clamper(mins, maxs);
    std::vector<float> output;
    output.reserve(objects_count_ * (current_umap_dim_ + 1));
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
    return plot_.get();
}

bool W2VHandler::IsReady() const {
    return ready_;
}

void W2VHandler::StartPrepare(const std::string& filename) {
    worker_ = std::make_unique<std::thread>([this, &filename]() {
        InitW2VEmbedding(filename);
        const constexpr size_t kDefaultDimension = 2;
        SetDimension(kDefaultDimension);
        ready_ = true;
        training_progress_ = 100.0f;
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
    if (!ready_) {
        // some logic when the data is not ready
        return;
    }
    if (is_first_time_ready_) {
        is_first_time_ready_ = false;
        plot_ = std::move(std::make_unique<Plot2DMesh>(plot_size_));
    }
    if (!is_data_loaded_) {
        LoadData();
    }
}