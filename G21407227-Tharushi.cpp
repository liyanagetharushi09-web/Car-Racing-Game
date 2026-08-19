// --------------------------------------------------------------
// CO1301 Games Concepts - Assignment 1 2025-26
// Name           : Tharushi Liyanage
// G Number       : G21407227
// Grade Expected : 70%+ (Milestone 4 - First classification)
// --------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <TL-Engine.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace tle;

// CONSTANTS 

// General values
const float kZero = 0.0f;
const float kHalf = 0.5f;
const int kZeroInt = 0;
const int kOne = 1;
const int kMinusOne = -1;

// Speed and physics
const float kSpeed = 1.0f;
const float kReverseDirection = -1.0f; // Used to flip velocity when bouncing (multiply by -1)
const float kBarrierBounceFactor = -0.5f; // Bounce off barriers is weaker (loses half speed, reversed)
const float kAimPrintThreshold = 0.1f; // Only reprint aim angle if it changes by at least 0.1°
const float kDegreesToRadians = 180.0f; // Conversion factor (angle * π / 180)

// Arena boundaries
const float kArenaFront = -20.0f;
const float kArenaBack = 200.0f;

// Camera setup
const float kCameraX = 0.0f;
const float kCameraY = 30.0f;
const float kCameraZ = -70.0f;
const float kCameraTiltDegrees = 15.0f;

// Floor and sky positions
const float kFloorX = 0.0f;
const float kFloorY = 0.0f;
const float kFloorZ = 0.0f;

const float kSkyX = 0.0f;
const float kSkyY = -900.0f; // Place skybox far below to cover horizon
const float kSkyZ = 0.0f;
const float kSkyboxHideY = -2000.0f; // Move old skybox far down to hide it
const float kSkyboxHideOffset = 1000.0f; // Extra offset when hiding skybox

// Blocks
const float kBlockSpacing = 13.0f;
const int kNumBlocksPerRow = 10;
const int kInitialBlockRows = 2;
const int kMaxBlockRows = 8;
const int kMaxBlocksTotal = kNumBlocksPerRow * kMaxBlockRows; // Total possible blocks in arena
const float kBlockRow1Z = 120.0f;
const float kBlockRow2Z = 135.0f;
const float kBlockY = 2.0f;

const float kBlockHalfWidth = 5.0f;
const float kBlockHalfHeight = 2.0f;
const float kBlockHalfDepth = 2.0f;

const float kBlockMoveSpeed = 0.02f; // Blocks creep forward each frame
const float kBlockSpawnDistance = 15.0f; // After moving this much, spawn a new row
const float kBlockSinkSpeed = 0.05f; // Speed at which blocks sink when destroyed
const float kBlockSinkDistance = 50.0f; // How far blocks sink before disappearing
const float kDeadY = -20.0f; // Y position below floor = "deleted"

// Barriers
const int kNumBarriers = 16;
const float kLeftBarrierX = -60.0f;
const float kRightBarrierX = 60.0f;
const float kBarrierZStart = 10.0f;
const float kBarrierZGap = 20.0f;
const float kBarrierY = 3.0f;

const float kBarrierHalfWidth = 1.0f;
const float kBarrierHalfHeight = 3.0f;
const float kBarrierHalfDepth = 5.0f;
const int kBarrierPairs = 2;

// Marbles
const int kNumMarbles = 3;
const float kMarbleRadius = 1.5f;
const float kMarbleY = 1.5f;
const float kMarbleFireSpeed = 0.3f;

const float kSlotX[kNumMarbles] = {-5.0f, 0.0f, 5.0f}; // Starting X positions for marbles
const float kSlotZ = -10.0f; // Starting Z position (behind firing line)
const float kFiringZ = 0.0f; // Z position when fired
const int kFirstMarbleIndex = 0;
const int kNoMarbleAvailable = -1; // Special value meaning "no marbles left"

// Aiming
const float kAimMin = -60.0f;
const float kAimMax = 60.0f;
const float kAimSpeed = 1.5f;
const float kForceReprintOffset = 999.0f;

// Game rules
const float kGameOverBlockZ = 5.0f; // If blocks reach this Z, game over
const int kMinLevel = 1;
const int kMaxLevels = 7;
const int kExitSuccess = 0;

// Boundary checks
const float kOutOfBoundsLeftX = -100.0f; // Marble too far left = reset
const float kOutOfBoundsRightX = 100.0f; // Marble too far right = reset

// Candidate mesh/skin filenames
const std::vector<const char*> floorCandidates =
{
    "Floor.x", "Ground.x", "Landscape.x", "Terrain.x"
};
const std::vector<const char*> skyCandidates =
{
    "Skybox_Hell.x", "Skybox.x", "Sky.x"
};
const std::vector<const char*> blockCandidates =
{
    "Block.x", "BlockA.x", "Block_B.x"
};
const std::vector<const char*> barrierCandidates =
{
    "Barrier.x", "BarrierA.x"
};
const std::vector<const char*> marbleCandidates =
{
    "Marble.x", "Sphere.x", "Ball.x"
};
const std::vector<const char*> arrowCandidates =
{
    "Arrow.x", "Pointer.x"
};

// Level-based skins for blocks - THEME PROGRESSION
const char* levelBlockSkins[] =
{
    "tiles.jpg", // Level 1 - Normal (Hell)
    "tiles_bright.jpg", // Level 2 - Bright (Fire Rising)
    "tiles_green.jpg", // Level 3 - Green (Flames)
    "tiles_half.jpg", // Level 4 - Half (Fire Intense)
    "tiles_half_brown.jpg", // Level 5 - Brown (Lava Pools)
    "tiles_pink.jpg", // Level 6 - Pink (Inferno)
    "tiles_red.jpg" // Level 7 - Red (Hellfire)
};

const char* kDamageSkin = "tiles_red.jpg";

// Level-based skyboxes - PROGRESSIVE HELL THEME
const char* levelSkyboxNames[] =
{
    "Skybox_Hell.x", // Level 1 - Hell
    "Skybox_Hell1.x", // Level 2 - Hell1 (Rising Flames)
    "Skybox_Hell2.x", // Level 3 - Hell2 (Fire Everywhere)
    "Skybox_Hell3.x", // Level 4 - Hell3 (Intense Heat)
    "Skybox_Hell4.x", // Level 5 - Hell4 (Lava)
    "Skybox_Hell5.x", // Level 6 - Hell5 (Deep Inferno)
    "Skybox_Hell6.x"  // Level 7 - Hell6 (Ultimate Hellfire)
};

// ENUMS/STRUCTS 

enum GameState {GameReady, GameFiring, GameContact, GameOver};

enum BlockState
{
    Untouched = 0, // Block not hit yet
    Hit = 1, // Block hit once (turns red)
    Sinking = 2, // Sinking into ground after final hit
    Deleted = 3 // Removed from game
};

enum MarbleState {Available, ReadyMarble, FiringMarble, ContactMarble, OverMarble};

struct Vector2D {float x; float z;};

// HELPER FUNCTIONS 
void TrySetSkin(IModel* model, const char* filename)
{
    if (model && filename) model->SetSkin(filename);
}

//multiple filenames until one works
IMesh* LoadFirstMesh(I3DEngine* engine, const std::vector<const char*>& candidates)
{
    if (!engine) return nullptr;
    for (auto name : candidates)
    {
        IMesh* m = engine->LoadMesh(name);
        if (m)
        {
            std::cout << "  -> Loaded: " << name << std::endl;
            return m;
        }
    }
    return nullptr;
}

// Load skybox for a specific level
IMesh* LoadLevelSkybox(I3DEngine* engine, int level)
{
    if (!engine || level < kMinLevel || level > kMaxLevels) return nullptr;

    const char* skyboxName = levelSkyboxNames[level - kMinLevel];
    std::cout << "Loading skybox for Level " << level << ": " << skyboxName << std::endl;

    IMesh* skybox = engine->LoadMesh(skyboxName);
    if (skybox)
    {
        return skybox;
    }
    return nullptr;
}

// Get block skin for a specific level (Returns the texture filename for the current level)
const char* GetLevelBlockSkin(int level)
{
    if (level < kMinLevel || level > kMaxLevels) level = kMinLevel;
    return levelBlockSkins[level - kMinLevel];
}

// Sphere-Box collision detection
bool SphereBoxCollision
(
    float sx, float sy, float sz, float radius,
    float bx, float by, float bz,
    float halfW, float halfH, float halfD,
    float& overlapX, float& overlapY, float& overlapZ
)
{
    float closestX = sx;
    float closestY = sy;
    float closestZ = sz;

    //Find closest point on the box to the sphere
    if (closestX < bx - halfW) closestX = bx - halfW;
    if (closestX > bx + halfW) closestX = bx + halfW;

    if (closestY < by - halfH) closestY = by - halfH;
    if (closestY > by + halfH) closestY = by + halfH;

    if (closestZ < bz - halfD) closestZ = bz - halfD;
    if (closestZ > bz + halfD) closestZ = bz + halfD;

    //Compute distance from sphere center to that closest point
    float dx = sx - closestX;
    float dy = sy - closestY;
    float dz = sz - closestZ;

    float distSq = dx * dx + dy * dy + dz * dz;

    if (distSq >= radius * radius) //Checks the collision
    {
        overlapX = overlapY = overlapZ = kZero;
        return false;
    }

    //Calculate Overlap
    overlapX = (halfW + radius) - fabsf(sx - bx);
    overlapY = (halfH + radius) - fabsf(sy - by);
    overlapZ = (halfD + radius) - fabsf(sz - bz);

    return true;
}

void ResetMarble(IModel* marble, int index, MarbleState& state)
{
    if (!marble) return;
    marble->SetPosition(kSlotX[index], kMarbleY, kSlotZ);
    state = Available;
}

int LoadNextMarble(MarbleState marbleStates[], int count,
    IModel* marbles[], IModel* firingDummy)
{
    for (int i = kFirstMarbleIndex; i < count; i++)
    {
        if (marbleStates[i] == Available)
        {
            float dummyX = firingDummy ? firingDummy->GetX() : kZero;
            if (marbles[i]) marbles[i]->SetPosition(dummyX, kMarbleY, kFiringZ);
            marbleStates[i] = ReadyMarble;
            std::cout << "Marble " << i << " is ready to fire!" << std::endl;
            return i;
        }
    }
    return kNoMarbleAvailable;
}

//Determines if the player has destroyed all blocks
bool CheckWinCondition(BlockState blockStates[], int activeCount)
{
    for (int i = kFirstMarbleIndex; i < activeCount; ++i)
    {
        if (blockStates[i] != Deleted) return false;
    }
    return true;
}

//Counts how many marbles are currently in play 
int CountActiveMarbles(MarbleState marbleStates[], int count)
{
    int active = kZeroInt;
    for (int i = kFirstMarbleIndex; i < count; ++i)
        if (marbleStates[i] != Available) ++active;
    return active;
}

//MAIN PROGRAM 
int main()
{
    I3DEngine* myEngine = New3DEngine(kTLX);
    myEngine->StartWindowed();

    myEngine->AddMediaFolder
    (
        "C:\\Users\\USER\\Desktop\\Personal\\Sftw. engineering degree\\"
        "1st year\\CO1301-Games Concepts\\Assignment 1 2025-26 models\\Media"
    );

    //LOAD MESHES
    IMesh* floorMesh = LoadFirstMesh(myEngine, floorCandidates);
    IModel* floorModel = floorMesh->CreateModel(kFloorX, kFloorY, kFloorZ);

    IMesh* skyMesh = LoadFirstMesh(myEngine, skyCandidates);
    IModel* skyboxModel = skyMesh->CreateModel(kSkyX, kSkyY, kSkyZ);

    ICamera* camera = myEngine->CreateCamera(kManual);
    camera->SetPosition(kCameraX, kCameraY, kCameraZ);
    camera->RotateLocalX(kCameraTiltDegrees);

    IMesh* blockMesh = LoadFirstMesh(myEngine, blockCandidates);
    IMesh* barrierMesh = LoadFirstMesh(myEngine, barrierCandidates);
    IMesh* marbleMesh = LoadFirstMesh(myEngine, marbleCandidates);
    IMesh* arrowMesh = LoadFirstMesh(myEngine, arrowCandidates);

    // BLOCKS - allocate arrays for maximum possible blocks
    IModel* blocks[kMaxBlocksTotal] = { nullptr };
    BlockState blockStates[kMaxBlocksTotal];
    int blockHitsRequired[kMaxBlocksTotal];  // Hits needed per block (based on level)
    int blockHitsTaken[kMaxBlocksTotal]; // Hits taken so far
    float blockInitialX[kMaxBlocksTotal];
    float blockSinkStartY[kMaxBlocksTotal];
    float blockInitialZ[kMaxBlocksTotal];

    float startX = -((kNumBlocksPerRow - kOne) * kHalf) * kBlockSpacing;

    // Current number of rows (start with 2 rows)    
    int currentRows = kInitialBlockRows;
    int currentNumBlocks = currentRows * kNumBlocksPerRow;

    // Distance between rows along Z-axis
    float rowGapZ = kBlockRow2Z - kBlockRow1Z;
    for (int r = kFirstMarbleIndex; r < currentRows; ++r)
    {
        float rowZ = kBlockRow1Z + r * rowGapZ; 

        //Loop through each column in the row
        for (int col = kFirstMarbleIndex; col < kNumBlocksPerRow; ++col)
        {
            int index = r * kNumBlocksPerRow + col; 
            float posX = startX + col * kBlockSpacing; 
            
            //Create block model at calculated position if mesh exists
            if (blockMesh)
                blocks[index] = blockMesh->CreateModel(posX, kBlockY, rowZ);
            
            blockInitialX[index] = posX;
            blockInitialZ[index] = rowZ;
            blockSinkStartY[index] = kBlockY;
            
            //Set block state and hit counters
            blockStates[index] = Untouched;
            blockHitsTaken[index] = kZeroInt;
            blockHitsRequired[index] = kMinLevel + kOne;
            TrySetSkin(blocks[index], GetLevelBlockSkin(kMinLevel));
        }
    }

    // BARRIERS
    IModel* barriers[kNumBarriers] = {nullptr};
    if (barrierMesh)
    {
        for (int i = kFirstMarbleIndex; i < kNumBarriers /kBarrierPairs; ++i)
        {
            float posZ = kBarrierZStart + i * kBarrierZGap;
            barriers[i] = barrierMesh->CreateModel(kLeftBarrierX, kBarrierY, posZ);
            barriers[i + kNumBarriers / kBarrierPairs] = barrierMesh->CreateModel(kRightBarrierX, kBarrierY, posZ);
        }
    }

    // MARBLES (3 total)
    IModel* marbles[kNumMarbles] = {nullptr};
    MarbleState marbleStates[kNumMarbles];
    Vector2D marbleVelocity[kNumMarbles];
    bool marbleGameOver[kNumMarbles];

    for (int i = kFirstMarbleIndex; i < kNumMarbles; ++i)
    {
        marbles[i] = marbleMesh->CreateModel(kSlotX[i], kMarbleY, kSlotZ);
        marbleVelocity[i].x = marbleVelocity[i].z = kZero;
        marbleStates[i] = (i == kFirstMarbleIndex) ? ReadyMarble : Available;
        marbleGameOver[i] = false;
    }
    int activeMarbleIndex = kFirstMarbleIndex;

    // FIRING DUMMY (Arrow)
    IModel* firingDummy = nullptr;
    if (arrowMesh) firingDummy = arrowMesh->CreateModel(kZero, kMarbleY, kFiringZ);

    float aimingAngle = kZero;

    // GAME STATE
    GameState gameState = GameReady;
    int level = kMinLevel;
    int score = kZeroInt;
    int prevScore = kNoMarbleAvailable;
    int prevLevel = kNoMarbleAvailable;

    float forwardAccum = kZero;

    // INITIALIZE LEVEL (called whenever the player advances to a new level)
    auto InitializeLevel = [&](int newLevel)
        {
            if (newLevel < kMinLevel || newLevel > kMaxLevels)
                newLevel = kMinLevel;

            level = newLevel;

            std::cout << "LEVEL " << level << " - DIFFICULTY INCREASED!\n";
            std::cout << "Blocks require " << (level + kOne) << " hit(s) to destroy\n";
            std::cout << "Block Texture: " << GetLevelBlockSkin(level) << std::endl;
            std::cout << "Skybox: " << levelSkyboxNames[level - kMinLevel] << std::endl;

            // Reset all blocks with new level settings
            for (int i = kFirstMarbleIndex; i < kMaxBlocksTotal; ++i)
            {
                if (blocks[i])
                {
                    blocks[i]->SetY(kDeadY - kSkyboxHideOffset);
                }
                blockStates[i] = Deleted;
                blockHitsTaken[i] = kZeroInt;
                blockHitsRequired[i] = level + kOne;
            }

            currentRows = kInitialBlockRows;
            currentNumBlocks = currentRows * kNumBlocksPerRow;

            for (int r = kZeroInt; r < currentRows; ++r)
            {
                float rowZ = kBlockRow1Z + r * rowGapZ;
                for (int col = kZeroInt; col < kNumBlocksPerRow; ++col)
                {
                    int index = r * kNumBlocksPerRow + col;
                    float posX = startX + col * kBlockSpacing;
                    if (!blocks[index] && blockMesh)
                        blocks[index] = blockMesh->CreateModel(posX, kBlockY, rowZ);
                    else if (blocks[index])
                        blocks[index]->SetPosition(posX, kBlockY, rowZ);

                    blockInitialX[index] = posX;
                    blockInitialZ[index] = rowZ;
                    blockSinkStartY[index] = kBlockY;
                    blockStates[index] = Untouched;
                    blockHitsTaken[index] = kZeroInt;
                    blockHitsRequired[index] = level + kOne;
                    TrySetSkin(blocks[index], GetLevelBlockSkin(level));
                }
            }

            // Update skybox
            IMesh* newSkyMesh = LoadLevelSkybox(myEngine, level);
            if (newSkyMesh && skyboxModel)
            {
                skyboxModel->SetY(kSkyboxHideY);
                skyboxModel = newSkyMesh->CreateModel(kSkyX, kSkyY, kSkyZ);
            }

            forwardAccum = kZero;
        };

    // RESET FUNCTION (This is called at the start of the game and whenever the player presses R to restart)
    auto ResetGame = [&]()
        {
            score = kZeroInt;
            level = kMinLevel;
            aimingAngle = kZero;

            if (firingDummy)
            {
                firingDummy->ResetOrientation();
                firingDummy->RotateY(aimingAngle);
                firingDummy->SetPosition(kZero, kMarbleY, kFiringZ);
            }

            // Reset marbles
            for (int m = kFirstMarbleIndex; m < kNumMarbles; ++m)
            {
                marbleVelocity[m].x = marbleVelocity[m].z = kZero;
                marbleGameOver[m] = false;
                if (marbles[m]) ResetMarble(marbles[m], m, marbleStates[m]);
                TrySetSkin(marbles[m], nullptr);
            }

            InitializeLevel(kMinLevel);

            activeMarbleIndex = LoadNextMarble(marbleStates, kNumMarbles, marbles, firingDummy);
            gameState = GameReady;

            std::cout << "Score: " << score
                << " | Level: " << level
                << " | Marbles: " << CountActiveMarbles(marbleStates, kNumMarbles)
                << "/" << kNumMarbles << "\n";
        };

    // MAIN GAME LOOP
    float lastPrintedAim = aimingAngle;

    while (myEngine->IsRunning())
    {
        if (!myEngine) break;

        myEngine->DrawScene();

        // Heads-Up Display output
        if (score != prevScore || level != prevLevel) //Only print when score/level changes
        {
            int activeMarblesCount = CountActiveMarbles(marbleStates, kNumMarbles);
            std::cout << "Score: " << score << " | Level: " << level
                << " | Hits needed per block: " << (level + kOne) << " | Marbles: "
                << activeMarblesCount << "/" << kNumMarbles << std::endl;
            prevScore = score; //Update previous values
            prevLevel = level;
        }

        // Quit
        if (myEngine->KeyHit(Key_Escape))
        {
            std::cout << "\nGame quit by player.\n";
            myEngine->Stop();
        }

        // Restart - ONLY in Ready state
        if (myEngine->KeyHit(Key_R) && gameState == GameReady)
        {
            ResetGame();
            lastPrintedAim = aimingAngle - kForceReprintOffset;
            continue;
        }

        if (gameState == GameOver) continue;

        // READY STATE
        if (gameState == GameReady)
        {
            // Aiming with X (left) and Z (right) keys
            if (myEngine->KeyHeld(Key_X))
            {
                aimingAngle -= kAimSpeed; //Rotate arrow left
                if (aimingAngle < kAimMin) aimingAngle = kAimMin;
                if (firingDummy)
                {
                    firingDummy->ResetOrientation();
                    firingDummy->RotateY(aimingAngle); //Apply rotation
                }
            }

            if (myEngine->KeyHeld(Key_Z))
            {
                aimingAngle += kAimSpeed; //Rotate arrow right
                if (aimingAngle > kAimMax) aimingAngle = kAimMax; //Clamp max angle
                if (firingDummy)
                {
                    firingDummy->ResetOrientation();
                    firingDummy->RotateY(aimingAngle);
                }
            }

            //This prevents spamming the console with the same aim value every frame
            if (fabs(aimingAngle - lastPrintedAim) >= kAimPrintThreshold)
            {
                std::cout << "Aim: " << aimingAngle << "°\n";
                lastPrintedAim = aimingAngle;
            }

            // SPACE to fire marble
            if (myEngine->KeyHit(Key_Space))
            {
                //If no marble is ready, load next one 
                if (activeMarbleIndex < kFirstMarbleIndex || activeMarbleIndex >= kNumMarbles ||
                    marbleStates[activeMarbleIndex] != ReadyMarble)
                {
                    activeMarbleIndex = LoadNextMarble(marbleStates, kNumMarbles, marbles, firingDummy);
                }

                if (activeMarbleIndex >= kFirstMarbleIndex)
                {
                    float angleRad = aimingAngle * (M_PI / kDegreesToRadians);

                    marbleVelocity[activeMarbleIndex].x = kMarbleFireSpeed * sinf(angleRad);
                    marbleVelocity[activeMarbleIndex].z = kMarbleFireSpeed * cosf(angleRad);

                    if (marbles[activeMarbleIndex])
                        marbles[activeMarbleIndex]->SetPosition
                        (
                            firingDummy ? firingDummy->GetX() : kZero, kMarbleY, kZero
                        );

                    marbleStates[activeMarbleIndex] = FiringMarble;
                    gameState = GameFiring;

                    std::cout << "FIRE! Marble " << activeMarbleIndex << " at " << aimingAngle << "°\n";
                }
            }
        }

        // FIRING/CONTACT STATE - Arrow can be rotated even while marble is in flight
        if (gameState == GameFiring || gameState == GameContact)
        {
            // Allow arrow aiming even while marble is in flight
            if (myEngine->KeyHeld(Key_X))
            {
                aimingAngle -= kAimSpeed;
                if (aimingAngle < kAimMin) aimingAngle = kAimMin;
                if (firingDummy)
                {
                    firingDummy->ResetOrientation();
                    firingDummy->RotateY(aimingAngle);
                }
            }

            if (myEngine->KeyHeld(Key_Z))
            {
                aimingAngle += kAimSpeed;
                if (aimingAngle > kAimMax) aimingAngle = kAimMax;
                if (firingDummy)
                {
                    firingDummy->ResetOrientation();
                    firingDummy->RotateY(aimingAngle);
                }
            }

            gameState = GameFiring;

            // Move blocks forward slowly and spawn rows when accumulated distance reached
            if (currentRows > kZeroInt)
            {
                forwardAccum += kBlockMoveSpeed;
                for (int b = kZeroInt; b < currentNumBlocks; ++b)
                {
                    if (blocks[b] && blockStates[b] != Deleted)
                        blocks[b]->MoveZ(-kBlockMoveSpeed);
                }

                // Animate sinking blocks as well
                for (int b = kZeroInt; b < currentNumBlocks; ++b)
                {
                    if (blockStates[b] == Sinking && blocks[b])
                    {
                        float by = blocks[b]->GetY();
                        blocks[b]->SetY(by - kBlockSinkSpeed);
                        // When it sinks enough, fully delete
                        if (blockSinkStartY[b] - blocks[b]->GetY() >= kBlockSinkDistance || blocks[b]->GetY() <= kDeadY)
                        {
                            blockStates[b] = Deleted;
                            blocks[b]->SetY(kDeadY);
                            ++score;
                            std::cout << "Block " << b << " removed after sinking. Score: " << score << "\n";
                        }
                    }
                }

                // Spawn new row at back if we've moved forward enough
                if (forwardAccum >= kBlockSpawnDistance && currentRows < kMaxBlockRows)
                {
                    int newRow = currentRows;
                    float newRowZ = kBlockRow2Z;
                    for (int col = kZeroInt; col < kNumBlocksPerRow; ++col)
                    {
                        int index = newRow * kNumBlocksPerRow + col;
                        float posX = startX + col * kBlockSpacing;
                        if (!blocks[index] && blockMesh)
                            blocks[index] = blockMesh->CreateModel(posX, kBlockY, newRowZ);
                        else if (blocks[index])
                            blocks[index]->SetPosition(posX, kBlockY, newRowZ);

                        blockInitialX[index] = posX;
                        blockInitialZ[index] = newRowZ;
                        blockSinkStartY[index] = kBlockY;
                        blockStates[index] = Untouched;
                        blockHitsTaken[index] = kZeroInt;
                        blockHitsRequired[index] = level + kOne;
                        TrySetSkin(blocks[index], GetLevelBlockSkin(level));
                    }
                    currentRows++;
                    currentNumBlocks = currentRows * kNumBlocksPerRow;
                    forwardAccum = kZero;
                    std::cout << "New row added at Z=" << newRowZ << ". Rows=" << currentRows << "\n";
                }
            }

            for (int m = kFirstMarbleIndex; m < kNumMarbles; ++m)
            {
                if (marbleStates[m] != FiringMarble && marbleStates[m] != ContactMarble) continue;

                // Move marble using velocity vectors
                if (marbles[m])
                {
                    marbles[m]->MoveX(marbleVelocity[m].x);
                    marbles[m]->MoveZ(marbleVelocity[m].z);
                }

                float mx = marbles[m] ? marbles[m]->GetX() : kZero;
                float my = marbles[m] ? marbles[m]->GetY() : kZero;
                float mz = marbles[m] ? marbles[m]->GetZ() : kZero;

                // Out of bounds at front - return to start position
                if (mz < kArenaFront)
                {
                    std::cout << "Marble " << m << " returned to start position\n";

                    if (marbles[m])
                    {
                        ResetMarble(marbles[m], m, marbleStates[m]);
                    }

                    int nextMarble = LoadNextMarble(marbleStates, kNumMarbles, marbles, firingDummy);

                    if (nextMarble >= kFirstMarbleIndex)
                    {
                        activeMarbleIndex = nextMarble;
                        gameState = GameReady;
                        std::cout << "Loaded next marble: " << nextMarble << "\n";
                    }
                    else
                    {
                        gameState = GameFiring;
                    }

                    continue;
                }

                // Marble reaches back of arena
                if (mz > kArenaBack)
                {
                    std::cout << "Marble" << m << " reached back of arena, returning to ready position\n";

                    if (marbles[m])
                    {
                        ResetMarble(marbles[m], m, marbleStates[m]);
                    }

                    int nextMarble = LoadNextMarble(marbleStates, kNumMarbles, marbles, firingDummy);

                    if (nextMarble >= kFirstMarbleIndex)
                    {
                        activeMarbleIndex = nextMarble;
                        gameState = GameReady;
                        std::cout << "Loaded next marble: " << nextMarble << "\n";
                    }
                    else
                    {
                        gameState = GameFiring;
                    }

                    continue;
                }

                // Out of bounds sideways
                if (mx < kOutOfBoundsLeftX || mx > kOutOfBoundsRightX)
                {
                    std::cout << "Marble " << m << " out of bounds at X=" << mx << "\n";

                    if (marbles[m])
                    {
                        ResetMarble(marbles[m], m, marbleStates[m]);
                    }

                    int nextMarble = LoadNextMarble(marbleStates, kNumMarbles, marbles, firingDummy);

                    if (nextMarble >= kFirstMarbleIndex)
                    {
                        activeMarbleIndex = nextMarble;
                        gameState = GameReady;
                    }
                    else
                    {
                        gameState = GameFiring;
                    }

                    continue;
                }

                // Block collisions
                for (int b = kFirstMarbleIndex; b < currentNumBlocks; ++b)
                {
                    if (blockStates[b] == Deleted) continue;
                    if (blockStates[b] == Sinking) continue;
                    if (!blocks[b]) continue;

                    float bx = blocks[b]->GetX();
                    float by = blocks[b]->GetY();
                    float bz = blocks[b]->GetZ();

                    float overlapX = kZero, overlapY = kZero, overlapZ = kZero;
                    bool hit = SphereBoxCollision(mx, my, mz, kMarbleRadius,
                        bx, by, bz,
                        kBlockHalfWidth, kBlockHalfHeight, kBlockHalfDepth,
                        overlapX, overlapY, overlapZ);

                    if (hit)
                    {
                        gameState = GameContact;

                        float absX = fabsf(overlapX);
                        float absY = fabsf(overlapY);
                        float absZ = fabsf(overlapZ);

                        // Reflect velocity vector based on collision surface
                        if (absX <= absY && absX <= absZ)
                        {
                            marbleVelocity[m].x *= kReverseDirection;
                        }
                        else if (absZ <= absY)
                        {
                            marbleVelocity[m].z *= kReverseDirection;
                        }
                        else
                        {
                            marbleVelocity[m].z *= kReverseDirection;
                        }

                        // Block hit logic
                        blockHitsTaken[b]++;

                        if (blockStates[b] == Untouched)
                        {
                            blockStates[b] = Hit;
                            if (blocks[b]) TrySetSkin(blocks[b], kDamageSkin);
                            std::cout << "Block " << b << " HIT! (" << blockHitsTaken[b] << "/" << blockHitsRequired[b] << ")\n";
                        }
                        else if (blockStates[b] == Hit)
                        {
                            blockStates[b] = Sinking;
                            blockSinkStartY[b] = blocks[b]->GetY();
                            std::cout << "Block " << b << " START SINKING (final hit)\n";
                        }

                        marbleStates[m] = FiringMarble;

                        // Separate marble from block to prevent multiple hits
                        if (marbles[m])
                        {
                            if (absX <= absY && absX <= absZ)
                                marbles[m]->SetX(mx + (kMarbleRadius + kBlockHalfWidth) * (marbleVelocity[m].x > kZero ? kOne : kMinusOne));
                            else if (absZ <= absY)
                                marbles[m]->SetZ(mz + (kMarbleRadius + kBlockHalfDepth) * (marbleVelocity[m].z > kZero ? kOne : kMinusOne));
                        }
                        break;
                    }
                }

                // Barrier collisions
                for (int br = kFirstMarbleIndex; br < kNumBarriers; ++br)
                {
                    if (!barriers[br]) continue;

                    float barX = barriers[br]->GetX();
                    float barY = barriers[br]->GetY();
                    float barZ = barriers[br]->GetZ();

                    float overlapX = kZero, overlapY = kZero, overlapZ = kZero;
                    bool hit = SphereBoxCollision
                    (
                        mx, my, mz, kMarbleRadius,
                        barX, barY, barZ,
                        kBarrierHalfWidth, kBarrierHalfHeight, kBarrierHalfDepth,
                        overlapX, overlapY, overlapZ
                    );

                    if (hit)
                    {
                        float absX = fabsf(overlapX);
                        float absZ = fabsf(overlapZ);

                        if (absX < absZ)
                            marbleVelocity[m].x *= kBarrierBounceFactor;
                        else
                            marbleVelocity[m].z *= kBarrierBounceFactor;

                        // Separate marble from barrier
                        if (marbles[m])
                        {
                            if (absX < absZ)
                                marbles[m]->SetX(mx + (kMarbleRadius + kBarrierHalfWidth) * (marbleVelocity[m].x > kZero ? kOne : kMinusOne));
                            else
                                marbles[m]->SetZ(mz + (kMarbleRadius + kBarrierHalfDepth) * (marbleVelocity[m].z > kZero ? kOne : kMinusOne));
                        }
                        break;
                    }
                }
            }
        }

        // CHECK FOR BLOCKS REACHING FRONT (game over) AND WIN CONDITION
        bool computerWon = false;
        for (int b = kZeroInt; b < currentNumBlocks; ++b)
        {
            if (blocks[b] && blockStates[b] != Deleted)
            {
                if (blocks[b]->GetZ() <= kGameOverBlockZ)
                {
                    computerWon = true;
                    break;
                }
            }
        }
        if (computerWon)
        {
            std::cout << "GAME OVER - COMPUTER WINS (blocks crossed front)\n";
            std::cout << "Final Score: " << score << "\n";

            for (int m = kZeroInt; m < kNumMarbles; ++m)
                TrySetSkin(marbles[m], kDamageSkin);

            gameState = GameOver;
        }

        // CHECK FOR LEVEL UP - ALL blocks must be Deleted
        if (gameState != GameOver && CheckWinCondition(blockStates, currentNumBlocks))
        {
            if (level < kMaxLevels)
            {
                gameState = GameOver;
                InitializeLevel(level + kOne);

                activeMarbleIndex = LoadNextMarble(marbleStates, kNumMarbles, marbles, firingDummy);
                if (activeMarbleIndex >= kFirstMarbleIndex) gameState = GameReady;
            }
            else
            {
                std::cout << "Completed all " << kMaxLevels << " levels!\n";
                std::cout << "Final Score: " << score << "\n";
                gameState = GameOver;
            }
        }

        // Auto-load next marble if none firing
        if (gameState == GameFiring)
        {
            bool anyFiring = false;
            for (int m = kFirstMarbleIndex; m < kNumMarbles; ++m)
            {
                if (marbleStates[m] == FiringMarble || marbleStates[m] == ContactMarble)
                {
                    anyFiring = true;
                    break;
                }
            }

            if (!anyFiring)
            {
                activeMarbleIndex = LoadNextMarble(marbleStates, kNumMarbles, marbles, firingDummy);
                if (activeMarbleIndex >= kFirstMarbleIndex)
                    gameState = GameReady;
                else
                    gameState = GameFiring;
            }
        }
    }
    myEngine->Delete();
    return kExitSuccess;
}