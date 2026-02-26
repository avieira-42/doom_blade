#ifndef INPUT_H
# define INPUT_H

int key_pressed(int keysym, void *arg);
int key_released(int keysym, void *arg);
int mouse_pressed(int keysym, void *arg);
int mouse_released(int keysym, void *arg);
int	mouse_move(int x, int y, void *arg);

#endif
