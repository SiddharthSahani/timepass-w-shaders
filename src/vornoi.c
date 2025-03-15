
#include <raylib/raylib.h>


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


typedef struct {
    Vector2 pos;
    Vector2 velocity;
    int uniLoc;
} Point;


Shader loadVornoiShader(int numPoints) {
    const char* fileContent = LoadFileText("shaders/vornoi.glsl");
    char* shaderSrc = TextReplace(fileContent, "<num-points>", TextFormat("%d", numPoints));
    Shader shader = LoadShaderFromMemory(0, shaderSrc);
    UnloadFileText(shaderSrc);
    return shader;
}


Point* createPoints(Shader shader, int numPoints, int velVariance) {
    Point* points = MemAlloc(sizeof(Point) * numPoints);
    for (int i = 0; i < numPoints; i++) {
        points[i].pos.x = GetRandomValue(0, GetScreenWidth());
        points[i].pos.y = GetRandomValue(0, GetScreenHeight());
        points[i].velocity.x = GetRandomValue(-velVariance, velVariance);
        points[i].velocity.y = GetRandomValue(-velVariance, velVariance);

        points[i].uniLoc = GetShaderLocation(shader, TextFormat("points[%d]", i));
    }
    return points;
}


void setPoints(Shader shader, Point* points, int numPoints) {
    for (int i = 0; i < numPoints; i++) {
        SetShaderValue(shader, points[i].uniLoc, &points[i].pos, SHADER_UNIFORM_VEC2);
    }
}


void updatePoints(Point* points, int numPoints) {
    for (int i = 0; i < numPoints; i++) {
        if (0 > points[i].pos.x || points[i].pos.x > GetScreenWidth()) {
            points[i].velocity.x *= -1;
        }
        if (0 > points[i].pos.y || points[i].pos.y > GetScreenHeight()) {
            points[i].velocity.y *= -1;
        }

        points[i].pos.x += points[i].velocity.x * GetFrameTime();
        points[i].pos.y += points[i].velocity.y * GetFrameTime();
    }
}


int main(int argc, const char **argv) {
    if (argc != 3) {
        TraceLog(LOG_ERROR, "Usage: ./vornoi <numPoints> <velocity-variance>");
        return 1;
    }
    int numPoints = TextToInteger(argv[1]);
    int velVariance = TextToInteger(argv[2]);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vornoi");
    SetTargetFPS(60);

    Shader shader = loadVornoiShader(numPoints);
    {
        int loc = GetShaderLocation(shader, "numPoints");
        SetShaderValue(shader, loc, &numPoints, SHADER_UNIFORM_INT);
    }

    Point* points = createPoints(shader, numPoints, velVariance);

    while (!WindowShouldClose()) {
        setPoints(shader, points, numPoints);
        updatePoints(points, numPoints);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginShaderMode(shader);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
        EndShaderMode();

        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadShader(shader);
    CloseWindow();

    return 0;
}
