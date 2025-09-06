/* points.c ... */

/*
 * This example creates an SDL window and renderer, and then draws some points
 * to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <iostream>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

/* (track everything as parallel arrays instead of a array of structs,
   so we can pass the coordinates to the renderer in a single function call.) */

/* Points are plotted as a set of X and Y coordinates.
   (0, 0) is the top left of the window, and larger numbers go down
   and to the right. This isn't how geometry works, but this is pretty
   standard in 2D graphics. */

/* marching squares stuffs */
// static int resolution = 10;
// #define WINDOW_WIDTH 600
// #define WINDOW_HEIGHT 400
// #define WINDOW_WIDTH 1280
// #define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define resolution 10
#define COLUMN (WINDOW_WIDTH / resolution)
#define ROW (WINDOW_HEIGHT / resolution)

// static int column = COLUMN;
// static int row = ROW;
int field[COLUMN + 1][ROW + 1];
int column = COLUMN + 1;
int row = ROW + 1;

typedef struct midpoints {
  float x;
  float y;
} midpoint;

void renderMidpoints (midpoint a, midpoint b) {
  SDL_RenderLine(renderer, a.x, a.y, b.x, b.y);
}

int verticies_arrangement(int a, int b, int c, int d) {
  if ( a == 0 && b == 0 && c == 0 && d == 0){
    return 1;
  } else if ( a == 0 && b == 0 && c == 0 && d == 1) {
    return 2;
  } else if ( a == 0 && b == 0 && c == 1 && d == 0) {
    return 3;
  } else if ( a == 0 && b == 0 && c == 1 && d == 1) {
    return 4;
  } else if ( a == 0 && b == 1 && c == 0 && d == 0) {
    return 5;
  } else if ( a == 0 && b == 1 && c == 0 && d == 1) {
    return 6;
  } else if ( a == 0 && b == 1 && c == 1 && d == 0) {
    return 7;
  } else if ( a == 0 && b == 1 && c == 1 && d == 1) {
    return 8;
  } else if ( a == 1 && b == 0 && c == 0 && d == 0) {
    return 8;
  } else if ( a == 1 && b == 0 && c == 0 && d == 1) {
    return 9;
  } else if ( a == 1 && b == 0 && c == 1 && d == 0) {
    return 6;
  } else if ( a == 1 && b == 0 && c == 1 && d == 1) {
    return 5;
  } else if ( a == 1 && b == 1 && c == 0 && d == 0) {
    return 4;
  } else if ( a == 1 && b == 1 && c == 0 && d == 1) {
    return 3;
  } else if ( a == 1 && b == 1 && c == 1 && d == 0) {
    return 2;
  } else if ( a == 1 && b == 1 && c == 1 && d == 1) {
    return 1;
  } 

  else return 0;
}

/* fps setup */
Uint32 start_time, frame_time;
float fps;

/* ellipses stuffs */
float radius = 750;
typedef struct focus_point {
  float x;
  float y;
} focus_p;

int stateFunction(float state){
  if(state > radius){
    return 0;
  } else return 1;
}

/* TODO: make a list of focus points */
float offset = 200;
focus_p f1 = { ((float) WINDOW_WIDTH) / 2 - offset, ((float) WINDOW_HEIGHT) / 2 };
focus_p f2 = { ((float) WINDOW_WIDTH) / 2 + offset, ((float) WINDOW_HEIGHT) / 2 };
focus_p f3 = { ((float) WINDOW_WIDTH) / 2 , ((float) WINDOW_HEIGHT) / 3 };

float dist(float x1, float y1, float x2, float y2){
  return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}



/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    int i, j;

    SDL_SetAppMetadata("Example Renderer Points", "1.0", "com.example.renderer-points");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/points", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

  /* setting up data for marching squares */

  float ii, jj;

  for (i = 0; i < column; i++) {
    for (j = 0; j < row; j++) {

      ii = ((float) i) * resolution;
      jj = ((float) j) * resolution;

      field[i][j] = dist(ii, jj, f1.x, f1.y) + dist(ii, jj, f2.x, f2.y) + dist(ii, jj, f3.x, f3.y);

//       if (dist(ii, jj, f1.x, f1.y) + dist(ii, jj, f2.x, f2.y + dist(ii, jj, f3.x, f3.y) > radius)){
//         field[i][j] = 0;
//       } else field[i][j] = 1;
    }
  }

    last_time = SDL_GetTicks();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

  const float step = 25;
  // TODO: add moving one of the focus points
  if (event->type == SDL_EVENT_KEY_DOWN) {

    if((event->key).key == SDLK_W)
    {
      std::cout << "pressed w" << std::endl;

      f1.y -= step;
    }
    if((event->key).key == SDLK_A)
    {
      std::cout << "pressed a" << std::endl;

      f1.x -= step;
    }
    if((event->key).key == SDLK_S)
    {
      std::cout << "pressed s" << std::endl;

      f1.y += step;
    }
    if((event->key).key == SDLK_D)
    {
      std::cout << "pressed d" << std::endl;

      f1.x += step;
    }

    std::cout << "outside the statement" << std::endl << std::endl;

    return SDL_APP_CONTINUE;
  }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{

  start_time = SDL_GetTicks();

  int i, j, state;
  float ii, jj;

  midpoint mid_ab, mid_ac, mid_bd, mid_cd;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
  SDL_RenderClear(renderer);  /* start with a blank canvas. */

//   SDL_RenderLine(renderer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  /* setting up data for marching squares */
  for (i = 0; i < column; i++) {
    for (j = 0; j < row; j++) {

      ii = ((float) i) * resolution;
      jj = ((float) j) * resolution;

      field[i][j] = ( dist(ii, jj, f1.x, f1.y) + dist(ii, jj, f2.x, f2.y) + dist(ii, jj, f3.x, f3.y) );

//       if (dist(ii, jj, f1.x, f1.y) + dist(ii, jj, f2.x, f2.y) + dist(ii, jj, f3.x, f3.y) > radius){
//         field[i][j] = 0;
//       } else {field[i][j] = 1;}
    }
  }

  /* look at the squares and draw appropriate lines in them */
  for(i = 0; i < column - 1; i++) {
    for(j = 0; j < row - 1; j++) {


      int a, b, c, d;

      a = stateFunction(field[i][j]);
      b = stateFunction(field[i+1][j]);
      c = stateFunction(field[i][j+1]);
      d = stateFunction(field[i+1][j+1]);

      state = verticies_arrangement(a, b, c, d);

      // function assigning list { field[][], ... } to a number
      /* state = verticies_arrangement(field[i][j], field[i+1][j], field[i][j+1], field[i+1][j+1]); */
//       std::cout << "field[" << i << "][" << j << "] = " << field[i][j] << std::endl;
//       std::cout << "field[" << i + 1 << "][" << j << "] = " << field[i + 1][j] << std::endl;
//       std::cout << "field[" << i << "][" << j + 1 << "] = " << field[i][j + 1] << std::endl;
//       std::cout << "field[" << i + 1 << "][" << j + 1 << "] = " << field[i + 1][j + 1] << std::endl;


      ii = ((float) i) * resolution;
      jj = ((float) j) * resolution;

      /* if(field[i][j] == 0){ */
        /* SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); */
        /* SDL_RenderPoint(renderer, ii, jj); */
        /* SDL_RenderLine(renderer, ii, jj, ii+resolution, jj+resolution); */
        /* SDL_RenderLine(renderer, ii, jj+resolution, ii+resolution, jj); */
      /* } else { */
        /* SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE); */
        /* SDL_RenderPoint(renderer, ii, jj); */
        /* SDL_RenderLine(renderer, ii, jj, ii+resolution, jj+resolution); */
        /* SDL_RenderLine(renderer, ii, jj+resolution, ii+resolution, jj); */
      /* } */

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

      mid_ab = {
        (1 - ((radius - field[i][j])/(field[i+1][j] - field[i][j]))) * ii + (((radius - field[i][j])/(field[i+1][j] - field[i][j]))) * (ii+resolution) ,
        jj
      };
      mid_ac = {
        ii, 
        (1 - ((radius - field[i][j])/(field[i][j+1] - field[i][j]))) * jj + (((radius - field[i][j])/(field[i][j+1] - field[i][j]))) * (jj+resolution) 
      };
      mid_bd = {
        ii + resolution,
        (1 - ((radius - field[i+1][j])/(field[i+1][j+1] - field[i+1][j]))) * jj + (((radius - field[i+1][j])/(field[i+1][j+1] - field[i+1][j]))) * (jj+resolution) 
      };
      mid_cd = {
        (1 - ((radius - field[i][j+1])/(field[i+1][j+1] - field[i][j+1]))) * ii + (((radius - field[i][j+1])/(field[i+1][j+1] - field[i][j+1]))) * (ii+resolution) ,
        jj + resolution
      };

      /* mid_ab = { (2 * ii + resolution) / raduis , jj                       }; */
      /* mid_ac = { ii                       , (2 * jj + resolution) / radius }; */
      /* mid_bd = { ii + resolution          , (2 * jj + resolution) / radius }; */
      /* mid_cd = { mid_ab.x                 , jj + resolution           }; */

      /* mid_ab = { (2 * ii + resolution) / 2, jj                        }; */
      /* mid_ac = { ii                       , (2 * jj + resolution) / 2 }; */
      /* mid_bd = { ii + resolution          , mid_ac.y                  }; */
      /* mid_cd = { mid_ab.x                 , jj + resolution           }; */

      switch (state){
        case 0:
          std::cout << "SDL_APP_FAILURE, mine error" << std::endl;
          return SDL_APP_FAILURE;
          break;
        case 1:
          break;
        case 2:
          /* renderMidpoints(mid_bd, mid_cd); */
          SDL_RenderLine(renderer, mid_cd.x, mid_cd.y, mid_bd.x, mid_bd.y);
          break;
        case 3:
          /* renderMidpoints(mid_ac, mid_cd); */
          SDL_RenderLine(renderer, mid_cd.x, mid_cd.y, mid_ac.x, mid_ac.y);
          break;
        case 4:
          /* renderMidpoints(mid_ac, mid_bd); */
          SDL_RenderLine(renderer, mid_ac.x, mid_ac.y, mid_bd.x, mid_bd.y);
          break;
        case 5:
          /* renderMidpoints(mid_ab, mid_bd); */
          SDL_RenderLine(renderer, mid_ab.x, mid_ab.y, mid_bd.x, mid_bd.y);
          break;
        case 6:
          /* renderMidpoints(mid_ab, mid_cd); */
          SDL_RenderLine(renderer, mid_cd.x, mid_cd.y, mid_ab.x, mid_ab.y);
          break;
        case 7:
          /* renderMidpoints(mid_ab, mid_bd); */
          /* renderMidpoints(mid_ac, mid_cd); */
          SDL_RenderLine(renderer, mid_ab.x, mid_ab.y, mid_bd.x, mid_bd.y);
          SDL_RenderLine(renderer, mid_cd.x, mid_cd.y, mid_ac.x, mid_ac.y);
          break;
        case 9:
          /* renderMidpoints(mid_ab, mid_ac); */
          /* renderMidpoints(mid_cd, mid_bd); */
          SDL_RenderLine(renderer, mid_ab.x, mid_ab.y, mid_ac.x, mid_ac.y);
          SDL_RenderLine(renderer, mid_cd.x, mid_cd.y, mid_bd.x, mid_bd.y);
          break;
        case 8:
          /* renderMidpoints(mid_ab, mid_ac); */
          SDL_RenderLine(renderer, mid_ab.x, mid_ab.y, mid_ac.x, mid_ac.y);
          break;
      }
    }
  }

//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
//     SDL_RenderPoints(renderer, points, SDL_arraysize(points));  /* draw all the points! */

    /* You can also draw single points with SDL_RenderPoint(), but it's
       cheaper (sometimes significantly so) to do them all at once. */

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

  SDL_FRect mark_f1 = { f1.x -3 , f1.y + 3, 6, 6 };
  SDL_FRect mark_f2 = { f2.x -3 , f2.y + 3, 6, 6 };
  SDL_FRect mark_f3 = { f3.x -3 , f3.y + 3, 6, 6 };

  SDL_RenderFillRect(renderer, &mark_f1);
  SDL_RenderFillRect(renderer, &mark_f2);
  SDL_RenderFillRect(renderer, &mark_f3);

  frame_time = SDL_GetTicks() - start_time;
  fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;

//   std::cout << fps << std::endl;

  /* garbage but will do for now */
  // FIXME: make the counter more legit
  char cfps[256];
  int ifps = (int) fps;
  sprintf(cfps, "%i", ifps);

  SDL_SetRenderScale(renderer, 2.0f, 2.0f);
  SDL_RenderDebugText(renderer, 10, 10, cfps);
  SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
