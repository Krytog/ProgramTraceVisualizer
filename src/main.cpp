#include <App/App.h>

#define DESIRED_FPS 60

int main() {
    const constexpr int kWidth = 1200;
    const constexpr int kHeight = 800;
    App app(kWidth, kHeight);
    app.Run<DESIRED_FPS>();
    return 0;
}