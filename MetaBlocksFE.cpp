#include <emscripten.h>
#include <emscripten/html5.h>
#include <cstdlib>
#include <ctime>

EMSCRIPTEN_KEEPALIVE
extern "C" {

int score = 100;
int box_x = 100;
int box_y = 100;
const int box_size = 50;
int canvas_width = 640;
int canvas_height = 480;

void reset_box() {
    box_x = rand() % (canvas_width - box_size);
    box_y = rand() % (canvas_height - box_size);
}

int check_click(int mouse_x, int mouse_y) {
    if (mouse_x >= box_x && mouse_x <= box_x + box_size &&
        mouse_y >= box_y && mouse_y <= box_y + box_size) {
        score++;
        reset_box();
        return 1;
    }
    return 0;
}

int get_score() {
    return score;
}

int get_box_x() {
    return box_x;
}

int get_box_y() {
    return box_y;
}

int get_box_size() {
    return box_size;
}

} // extern "C"
