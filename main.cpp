/**

* Author: Jiaxin Lin

* Assignment: Pong Clone

* Date due: 10/16/2026

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

#include "cs3113.h"
#include <math.h>

// Global Constants
constexpr int SCREEN_WIDTH  = 1600 / 2,
              SCREEN_HEIGHT = 900 / 2,
              FPS           = 60,
              SPEED         = 200,
              WIN_SCORE     = 3;
constexpr float BALL_ROTATION_SPEED = 40.0f,
                WINNER_MOVE_SPEED   = 120.0f,
                WINNER_GROW_SPEED   = 35.0f;
constexpr char BG_COLOUR[] = "#F8F1C8";
// Image downloaded from Google Image
constexpr char PLAYER1_FP[] = "Player1.png";
constexpr char PLAYER2_FP[] = "Player2.png";
constexpr char BALL_FP[]    = "Cheese.png";
constexpr Vector2 PLAYER1_Origin = {45.0f, SCREEN_HEIGHT / 2.0f};
constexpr Vector2 PLAYER2_Origin = {SCREEN_WIDTH - 45.0f, SCREEN_HEIGHT / 2.0f};
constexpr Vector2 BALL_Origin = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f,
      gBallAngle     = 0.0f;
int gPlayer1Score = 0,
    gPlayer2Score = 0,
    gBallCount    = 1;
bool gGameOver     = false;
bool gSinglePlayer = false;
Vector2 gPlayer1Position = PLAYER1_Origin,
        gPlayer1Movement = { 0.0f, 0.0f },
        gPlayer1Scale    = { 55.0f, 90.0f },

        gPlayer2Position = PLAYER2_Origin,
        gPlayer2Movement = { 0.0f, 0.0f },
        gPlayer2Scale    = { 55.0f, 90.0f },

        gBall1Position   = BALL_Origin,
        gBall1Movement   = { 1.0f, 1.0f },

        gBall2Position   = BALL_Origin,
        gBall2Movement   = { -1.0f, 1.0f },

        gBall3Position   = BALL_Origin,
        gBall3Movement   = { 1.0f, -1.0f },

        gBallScale       = { 60.0f, 60.0f };

Texture2D gPlayer1Texture;
Texture2D gPlayer2Texture;
Texture2D gBallTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
void renderObject(const Texture2D *texture, const Vector2 *position, const Vector2 *scale, float angle);
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);


// Function Definitions
void renderObject(const Texture2D *texture, const Vector2 *position, const Vector2 *scale, float angle){
    Rectangle textureArea = {
        0.0f,
        0.0f,
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    DrawTexturePro(
        *texture,
        textureArea,
        destinationArea,
        originOffset,
        angle,
        WHITE
    );
}


bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB){
    float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);

    float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tom and Jerry Pong");

    gPlayer1Texture = LoadTexture(PLAYER1_FP);
    gPlayer2Texture = LoadTexture(PLAYER2_FP);
    gBallTexture    = LoadTexture(BALL_FP);

    SetTargetFPS(FPS);
}


void processInput()
{
    gPlayer1Movement = { 0.0f, 0.0f };
    gPlayer2Movement = { 0.0f, 0.0f };

    // Switch game mode
    if (IsKeyPressed(KEY_T)){
        gSinglePlayer = !gSinglePlayer;
    }

    // Change number of balls
    if (IsKeyPressed(KEY_ONE)){
        gBallCount = 1;
    }

    if (IsKeyPressed(KEY_TWO)){
        gBallCount = 2;
    }

    if (IsKeyPressed(KEY_THREE)){
        gBallCount = 3;
    }

    if (!gGameOver){
        if      (IsKeyDown(KEY_W)) gPlayer1Movement.y = -1.0f;
        else if (IsKeyDown(KEY_S)) gPlayer1Movement.y =  1.0f;

        if (!gSinglePlayer)
        {
            if      (IsKeyDown(KEY_UP))   gPlayer2Movement.y = -1.0f;
            else if (IsKeyDown(KEY_DOWN)) gPlayer2Movement.y =  1.0f;
        }
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
    {
        gAppStatus = TERMINATED;
    }
}


void update()
{
    // Delta time
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;


    // Game Over animation
    if (gGameOver){
        // Player1 wins
        if (gPlayer1Score >= WIN_SCORE){
            if (gPlayer1Position.x < SCREEN_WIDTH / 2.0f){
                gPlayer1Position.x += WINNER_MOVE_SPEED * deltaTime;
            }
            if (gPlayer1Position.y < SCREEN_HEIGHT / 2.0f){
                gPlayer1Position.y += WINNER_MOVE_SPEED * deltaTime;
            }
            else if (gPlayer1Position.y > SCREEN_HEIGHT / 2.0f){
                gPlayer1Position.y -= WINNER_MOVE_SPEED * deltaTime;
            }
            if (gPlayer1Scale.x < 150.0f){
                gPlayer1Scale.x += WINNER_GROW_SPEED * deltaTime;
                gPlayer1Scale.y += WINNER_GROW_SPEED * 1.5f * deltaTime;
            }
        }

        // Player2 wins
        else if (gPlayer2Score >= WIN_SCORE){
            if (gPlayer2Position.x > SCREEN_WIDTH / 2.0f){
                gPlayer2Position.x -= WINNER_MOVE_SPEED * deltaTime;
            }
            if (gPlayer2Position.y < SCREEN_HEIGHT / 2.0f){
                gPlayer2Position.y += WINNER_MOVE_SPEED * deltaTime;
            }
            else if (gPlayer2Position.y > SCREEN_HEIGHT / 2.0f){
                gPlayer2Position.y -= WINNER_MOVE_SPEED * deltaTime;
            }
            if (gPlayer2Scale.x < 150.0f){
                gPlayer2Scale.x += WINNER_GROW_SPEED * deltaTime;
                gPlayer2Scale.y += WINNER_GROW_SPEED * 1.5f * deltaTime;
            }
        }

        return;
    }


    // Rotate the cheese
    gBallAngle += BALL_ROTATION_SPEED * deltaTime;

    if (gBallAngle >= 360.0f){
        gBallAngle = 0.0f;
    }

    // Single-player
    if (gSinglePlayer){
        if (gBall1Position.y < gPlayer2Position.y){
            gPlayer2Movement.y = -1.0f;
        }
        else if (gBall1Position.y > gPlayer2Position.y){
            gPlayer2Movement.y = 1.0f;
        }
    }

    // Player movement
    gPlayer1Position.y += SPEED * gPlayer1Movement.y * deltaTime;
    gPlayer2Position.y += SPEED * gPlayer2Movement.y * deltaTime;

    // Player 1 boundary
    if (gPlayer1Position.y - gPlayer1Scale.y / 2.0f < 0.0f){
        gPlayer1Position.y = gPlayer1Scale.y / 2.0f;
    }

    if (gPlayer1Position.y + gPlayer1Scale.y / 2.0f > SCREEN_HEIGHT){
        gPlayer1Position.y =
            SCREEN_HEIGHT - gPlayer1Scale.y / 2.0f;
    }

    // Player 2 boundary
    if (gPlayer2Position.y - gPlayer2Scale.y / 2.0f < 0.0f){
        gPlayer2Position.y = gPlayer2Scale.y / 2.0f;
    }

    if (gPlayer2Position.y + gPlayer2Scale.y / 2.0f > SCREEN_HEIGHT){
        gPlayer2Position.y =
            SCREEN_HEIGHT - gPlayer2Scale.y / 2.0f;
    }


    // BALL 1

    gBall1Position.x += SPEED * gBall1Movement.x * deltaTime;
    gBall1Position.y += SPEED * gBall1Movement.y * deltaTime;

    if (gBall1Position.y - gBallScale.y / 2.0f <= 0.0f){
        gBall1Position.y = gBallScale.y / 2.0f;
        gBall1Movement.y *= -1.0f;
    }
    if (gBall1Position.y + gBallScale.y / 2.0f >= SCREEN_HEIGHT){
        gBall1Position.y =
            SCREEN_HEIGHT - gBallScale.y / 2.0f;

        gBall1Movement.y *= -1.0f;
    }
    if (isColliding(&gBall1Position, &gBallScale, &gPlayer1Position, &gPlayer1Scale)){
        gBall1Position.x =
            gPlayer1Position.x
            + gPlayer1Scale.x / 2.0f
            + gBallScale.x / 2.0f;

        gBall1Movement.x *= -1.0f;
    }

    if (isColliding(&gBall1Position, &gBallScale, &gPlayer2Position, &gPlayer2Scale)){
        gBall1Position.x =
            gPlayer2Position.x
            - gPlayer2Scale.x / 2.0f
            - gBallScale.x / 2.0f;

        gBall1Movement.x *= -1.0f;
    }

    if (gBall1Position.x - gBallScale.x / 2.0f > SCREEN_WIDTH){
        gPlayer1Score++;

        gBall1Position = BALL_Origin;
        gBall1Movement = { -1.0f, 1.0f };
    }

    if (gBall1Position.x + gBallScale.x / 2.0f < 0.0f){
        gPlayer2Score++;

        gBall1Position = BALL_Origin;
        gBall1Movement = { 1.0f, 1.0f };
    }


    // BALL 2

    if (gBallCount >= 2){
        gBall2Position.x += SPEED * gBall2Movement.x * deltaTime;
        gBall2Position.y += SPEED * gBall2Movement.y * deltaTime;

        if (gBall2Position.y - gBallScale.y / 2.0f <= 0.0f){
            gBall2Position.y = gBallScale.y / 2.0f;
            gBall2Movement.y *= -1.0f;
        }

        if (gBall2Position.y + gBallScale.y / 2.0f >= SCREEN_HEIGHT){
            gBall2Position.y =
                SCREEN_HEIGHT - gBallScale.y / 2.0f;

            gBall2Movement.y *= -1.0f;
        }

        if (isColliding(&gBall2Position, &gBallScale, &gPlayer1Position, &gPlayer1Scale)){
            gBall2Position.x =
                gPlayer1Position.x
                + gPlayer1Scale.x / 2.0f
                + gBallScale.x / 2.0f;

            gBall2Movement.x *= -1.0f;
        }

        if (isColliding(&gBall2Position, &gBallScale, &gPlayer2Position, &gPlayer2Scale)){
            gBall2Position.x =
                gPlayer2Position.x
                - gPlayer2Scale.x / 2.0f
                - gBallScale.x / 2.0f;

            gBall2Movement.x *= -1.0f;
        }

        if (gBall2Position.x - gBallScale.x / 2.0f > SCREEN_WIDTH){
            gPlayer1Score++;
            gBall2Position = BALL_Origin;
            gBall2Movement = { -1.0f, -1.0f };
        }

        if (gBall2Position.x + gBallScale.x / 2.0f < 0.0f){
            gPlayer2Score++;
            gBall2Position = BALL_Origin;
            gBall2Movement = { 1.0f, -1.0f };
        }
    }


    // BALL 3

    if (gBallCount >= 3){
        gBall3Position.x += SPEED * gBall3Movement.x * deltaTime;
        gBall3Position.y += SPEED * gBall3Movement.y * deltaTime;

        if (gBall3Position.y - gBallScale.y / 2.0f <= 0.0f){
            gBall3Position.y = gBallScale.y / 2.0f;
            gBall3Movement.y *= -1.0f;
        }

        if (gBall3Position.y + gBallScale.y / 2.0f >= SCREEN_HEIGHT){
            gBall3Position.y =
                SCREEN_HEIGHT - gBallScale.y / 2.0f;

            gBall3Movement.y *= -1.0f;
        }

        if (isColliding(&gBall3Position, &gBallScale, &gPlayer1Position, &gPlayer1Scale)){
            gBall3Position.x =
                gPlayer1Position.x
                + gPlayer1Scale.x / 2.0f
                + gBallScale.x / 2.0f;

            gBall3Movement.x *= -1.0f;
        }

        if (isColliding(&gBall3Position, &gBallScale, &gPlayer2Position, &gPlayer2Scale)){
            gBall3Position.x =
                gPlayer2Position.x
                - gPlayer2Scale.x / 2.0f
                - gBallScale.x / 2.0f;

            gBall3Movement.x *= -1.0f;
        }

        if (gBall3Position.x - gBallScale.x / 2.0f > SCREEN_WIDTH){
            gPlayer1Score++;
            gBall3Position = BALL_Origin;
            gBall3Movement = { -1.0f, 1.0f };
        }

        if (gBall3Position.x + gBallScale.x / 2.0f < 0.0f){
            gPlayer2Score++;
            gBall3Position = BALL_Origin;
            gBall3Movement = { 1.0f, 1.0f };
        }
    }


    // Game Over
    if (gPlayer1Score >= WIN_SCORE || gPlayer2Score >= WIN_SCORE){
        gGameOver = true;
    }
}


void render()
{
    BeginDrawing();

    ClearBackground(ColorFromHex(BG_COLOUR));


    // Center line
    for (int y = 0; y < SCREEN_HEIGHT; y += 30)
    {
        DrawRectangle(
            SCREEN_WIDTH / 2 - 2,
            y,
            4,
            15,
            LIGHTGRAY
        );
    }


    if (!gGameOver)
    {
        // Controls
        DrawText(
            "P1: W / S",
            15,
            15,
            18,
            BLACK
        );

        DrawText(
            "P2: UP / DOWN",
            SCREEN_WIDTH - 145,
            15,
            18,
            BLACK
        );


        // Score
        DrawText(
            TextFormat("%d", gPlayer1Score),
            SCREEN_WIDTH / 2 - 80,
            15,
            35,
            BLACK
        );

        DrawText(
            TextFormat("%d", gPlayer2Score),
            SCREEN_WIDTH / 2 + 60,
            15,
            35,
            BLACK
        );


        // Mode
        if (gSinglePlayer)
        {
            DrawText(
                "SINGLE PLAYER",
                SCREEN_WIDTH / 2 - 70,
                58,
                18,
                BLACK
            );
        }

        else
        {
            DrawText(
                "TWO PLAYER",
                SCREEN_WIDTH / 2 - 58,
                58,
                18,
                BLACK
            );
        }


        // Number of balls
        DrawText(
            TextFormat("BALLS: %d", gBallCount),
            SCREEN_WIDTH / 2 - 40,
            82,
            18,
            BLACK
        );


        DrawText(
            "T: MODE     1 / 2 / 3: BALLS",
            SCREEN_WIDTH / 2 - 115,
            SCREEN_HEIGHT - 25,
            16,
            DARKGRAY
        );

        // Player 1
        renderObject(&gPlayer1Texture,&gPlayer1Position,&gPlayer1Scale,0.0f);

        // Player 2
        renderObject(&gPlayer2Texture, &gPlayer2Position, &gPlayer2Scale,0.0f);

        // Ball 1
        renderObject(&gBallTexture, &gBall1Position, &gBallScale, gBallAngle);

        // Ball 2
        if (gBallCount >= 2){
            renderObject(&gBallTexture,&gBall2Position,&gBallScale,gBallAngle);
        }


        // Ball 3
        if (gBallCount >= 3){
            renderObject(&gBallTexture,&gBall3Position,&gBallScale,gBallAngle
            );
        }
    }

    // Game Over
    if (gGameOver)
    {
        // Player 1 = Jerry
        if (gPlayer1Score >= WIN_SCORE)
        {
            DrawText(
                "JERRY WINS!",
                SCREEN_WIDTH / 2 - 115,
                55,
                40,
                BLACK
            );

            renderObject(
                &gPlayer1Texture,
                &gPlayer1Position,
                &gPlayer1Scale,
                0.0f
            );
        }

        // Player 2 = Tom
        else if (gPlayer2Score >= WIN_SCORE)
        {
            DrawText(
                "TOM WINS!",
                SCREEN_WIDTH / 2 - 100,
                55,
                40,
                BLACK
            );

            renderObject(
                &gPlayer2Texture,
                &gPlayer2Position,
                &gPlayer2Scale,
                0.0f
            );
        }

        DrawText(
            "Press Q to Quit",
            SCREEN_WIDTH / 2 - 70,
            SCREEN_HEIGHT - 40,
            18,
            DARKGRAY
        );
    }


    EndDrawing();
}


void shutdown()
{
    UnloadTexture(gPlayer1Texture);
    UnloadTexture(gPlayer2Texture);
    UnloadTexture(gBallTexture);

    CloseWindow();
}


int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
