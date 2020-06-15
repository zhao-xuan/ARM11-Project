#include "game.h"


void startGame(int levelNum) {
  WINDOW *game_win = init_display();
  state_t *curr = initState(levelNum);
  bool hasMoving = false;

  timespec_t now, lastFrame;
  unsigned long frameNum = 0;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastFrame);

  while (1) {
    updateFrame(&now, &lastFrame, &frameNum);

    if (!hasMoving) {
      if (!spawnTetriminos(curr)) break;
      hasMoving = true;
    }
    printState(curr, game_win);
    int key = getInput();
    processInput(curr, key);

    if (frameNum % framePerDrop(curr->level) == 0) hasMoving = dropPiece(curr);
  }

  freeState(curr);
  endwin();
  
  if (curr->level.score > curr->highScore) writeHighScore(curr->level.score);
  printf("You scored %d points. \n", curr->level.score);
  printf("The high score is %d points. \n", readHighScore());
}


void updateFrame(timespec_t *now, timespec_t *lastFrame,
                 unsigned long *frameNum) {
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, now);
  double delta = (now->tv_sec - lastFrame->tv_sec) * 1e6 +
                 (now->tv_nsec - lastFrame->tv_nsec) / 1e3;
  *lastFrame = *now;
  if (delta < 1e6 / FRAME_RATE) {
    usleep(1e6 / FRAME_RATE - delta);
  }
  *frameNum = *frameNum + 1;
}
