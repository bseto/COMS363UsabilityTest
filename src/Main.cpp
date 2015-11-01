//for the rasperry pi
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
extern "C" {
    #include <SDL.h>
    #include <SDL_ttf.h>
}
#include <stdio.h>
#include <unistd.h>
#include <SDL_image.h>

#define SCREEN_HEIGHT 1440
#define SCREEN_WIDTH 2560

void processEvents();
void signalToQuit();
void close();

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;

SDL_Rect videoRect;
SDL_Rect musicBarRect;
int quit;

SDL_Texture* gkidsport_homepage = NULL; 

/***********************************************************************
/*                          SDL functions
/***********************************************************************/
// Initializes SDL window / renderer for use

bool init_SDL()
{
    bool success = true;

    if (SDL_Init(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        window = SDL_CreateWindow("Video Application", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                printf("Renderer could not be created. SDL_Error: %s \n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    return success;
}



void processEvents()
{
    SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    printf("SDL_QUIT was called\n");
                    close();
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            printf("Esc was Pressed!\n");
                            close();
                            break;
                        case SDLK_LEFT:
                            printf("Left arrow was Pressed!\n");
                            break;
                        case SDLK_RIGHT:
                            printf("Right arrow was Pressed!\n");
                            break;
                        case SDLK_UP:
                            break;
                        case SDLK_DOWN:
                            break;
                        case SDLK_SPACE:
                            printf("Space was pressed!\n");
                            SDL_RenderCopy(renderer, gkidsport_homepage, NULL, NULL );
                            SDL_RenderPresent(renderer);
                            usleep(50000); //usleep is in microseconds. 1000 microseconds = 1 millisecond
                            break;

                    }
            }
        }
}

SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}
/* Cleans up and should free everything used in the program*/
void close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    SDL_Quit();
    exit(0);
}

int main(int argc, char* argv[])
{
    if (!init_SDL())
    {
        fprintf(stderr, "Could not initialize SDL!\n");
        return -1;
    }

    gkidsport_homepage = loadTexture("assets/homepage.png");
    if (gkidsport_homepage == NULL)
    {
        printf("ERROR loading the .png");
        return -1;
    }

    while (!quit)
    {
        processEvents();

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderPresent(renderer);
        usleep(4000);
    }

    return 0;
}
