#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_pos_x = 0.f;
float player_pos_y = 0.f;

// .3f - 0.3 per frame. 
// BUT bigger frame slower movement! 
// We need FPS - Frames Per Seconds

internal void
simulate_game(Input* input, float dt) {
    clear_screen(0xfff7d5);

    float speed = 50.f; // uniots per seconds
    if (is_down(BUTTON_UP)) player_pos_y += speed * dt;
    if (is_down(BUTTON_DOWN)) player_pos_y -= speed * dt;
    if (is_down(BUTTON_LEFT)) player_pos_x -= speed * dt;
    if (is_down(BUTTON_RIGHT)) player_pos_x += speed * dt;
       
    draw_rect(player_pos_x, player_pos_y, 1, 1, 0x090979);
    draw_rect(20, 20, 5, 1, 0xffe370);
    draw_rect(-40, -10, 2, 6, 0xffe370);
}