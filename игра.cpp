#include <iostream>
#include <conio.h>
#include <vector>
#include <string>

using namespace std;

enum BlockType {
    WALL,
    GRASS,
    DOOR, 
    BOMB,
    SWORD,
    HEALTH_PACK
};

enum EnemyType {
    MELEE,
    RANGED,
    TANK
};


struct Block {
    BlockType type;
};


struct Player {
    int x, y;
    int health = 100;
    int meleeDamage = 10;
    int rangedDamage = 20;
    int tankDamage = 30;
    int points = 0 ;
    bool hasSword = false; 
    int swordStrength = 10; 
    int swordRemainingHits = 0; 

    void equipSword() {
        hasSword = true;
        swordRemainingHits = 3; 
    }



    int getMeleeDamage() const {
        return meleeDamage;
    }

    int getTankDamage() const {
        return tankDamage;
    }

    int getRangedDamage() const {
        return rangedDamage;
    }

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }

    void addPoints(int amount) {
        points += amount;
    }
};

struct Enemy {
    int x, y;
    EnemyType type;
    int health;
    int meleeDamage = 5;
    int rangedDamage = 7;
    int tankDamage = 10;

    int getMeleeDamage() const {
        return meleeDamage;
    }

    int getTankDamage() const {
        return tankDamage;
    }

    int getRangedDamage() const {
        return rangedDamage;
    }

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }
};

void meleeCombat(Player& player, Enemy& enemy) {
    int playerDamage = player.getMeleeDamage();
    int enemyDamage = enemy.getMeleeDamage();

    while (player.isAlive() && enemy.isAlive()) {
        enemy.takeDamage(playerDamage);
        if (enemy.isAlive()) {
            player.takeDamage(enemyDamage);
        }
    }

    if (player.isAlive()) {
        player.addPoints(10); 
        std::cout << "Player won. Points earned: " << player.points << std::endl;
        enemy.health = 0; 
    }
    else {
        std::cout << "Player lost. Points earned: " << player.points << std::endl;

    }
}


void rangedCombat(Player& player, Enemy& enemy) {
    int playerDamage = player.getRangedDamage();
    int enemyDamage = enemy.getRangedDamage();

    while (player.isAlive() && enemy.isAlive()) {
        enemy.takeDamage(playerDamage);
        if (enemy.isAlive()) {
            player.takeDamage(enemyDamage);
        }
    }

    if (player.health > 0) {
        player.addPoints(30); 
    }
    else {
        std::cout << "Player lost. Points earned: " << player.points << std::endl;
    }
}

void tankCombat(Player& player, Enemy& enemy) {
    int playerDamage = player.getTankDamage();
    int enemyDamage = enemy.getTankDamage();

    while (player.isAlive() && enemy.isAlive()) {
        enemy.takeDamage(playerDamage);
        if (enemy.isAlive()) {
            player.takeDamage(enemyDamage);
        }
    }

    if (player.health > 0) {
        player.addPoints(50);
    }
    else {
        std::cout << "Player lost. Points earned: " << player.points << std::endl;
    }
}
void moveEnemy(Enemy& enemy, Block gameMap[10][20]) {
    int direction = rand() % 4; 
    int newX = enemy.x;
    int newY = enemy.y;

    switch (direction) {
    case 0:
        newY--;
        break;
    case 1: 
        newY++;
        break;
    case 2: 
        newX--;
        break;
    case 3:
        newX++;
        break;
    }

    if (gameMap[newY][newX].type != WALL) {
        enemy.x = newX;
        enemy.y = newY;
    }
}

void moveEnemyMelee(Enemy& enemy, Player& player, Block gameMap[10][20], int radius=1) {
    if (std::abs(player.x - enemy.x) <= radius && std::abs(player.y - enemy.y) <= radius) {
        player.takeDamage(enemy.meleeDamage);
    }
    else {
        int direction = rand() % 4;
        int newX = enemy.x;
        int newY = enemy.y;

        switch (direction) {
        case 0: 
            newY--;
            break;
        case 1:
            newY++;
            break;
        case 2: 
            newX--;
            break;
        case 3: 
            newX++;
            break;
        }

        if (gameMap[newY][newX].type != WALL) {
            enemy.x = newX;
            enemy.y = newY;
        }
    }
}

void moveEnemyRanged(Enemy& enemy, Player& player, Block gameMap[10][20], int radius=2) {
    if (std::abs(player.x - enemy.x) <= radius && std::abs(player.y - enemy.y) <= radius) {
        player.takeDamage(enemy.rangedDamage);
    }
    else {
        int direction = rand() % 4; 
        int newX = enemy.x;
        int newY = enemy.y;


        switch (direction) {
        case 0:
            newY--;
            break;
        case 1:
            newY++;
            break;
        case 2:
            newX--;
            break;
        case 3:
            newX++;
            break;
        }

        if (gameMap[newY][newX].type != WALL) {
            enemy.x = newX;
            enemy.y = newY;
        }
    }
}

void moveEnemyTank(Enemy& enemy, Player& player, Block gameMap[10][20], int& moveCounter, int radius=3) {
    if (moveCounter % 2 == 0) {
        if (std::abs(player.x - enemy.x) <= radius && std::abs(player.y - enemy.y) <= radius) {
            player.takeDamage(enemy.tankDamage);
        }
        else {
            int direction = rand() % 4;
            int newX = enemy.x;
            int newY = enemy.y;

            switch (direction) {
            case 0:
                newY--;
                break;
            case 1:
                newY++;
                break;
            case 2:
                newX--;
                break;
            case 3:
                newX++;
                break;
            }

            if (gameMap[newY][newX].type != WALL) {
                enemy.x = newX;
                enemy.y = newY;
            }
        }
    }
    moveCounter++;
}



void drawBlock(Block block) {
    switch (block.type) {
    case WALL:
        cout << "#";
        break;
    case GRASS:
        cout << ".";
        break;
    case DOOR:
        cout << "D";
        break;
    case BOMB:
        cout << "B";
        break;
    case SWORD:
        cout << "S";
        break;
    case HEALTH_PACK:
        cout << "H";
        break;
    }
}

void drawLegend() {
    cout << "Legend:" << endl;
    cout << "# : Wall" << endl;
    cout << ". : Grass" << endl;
    cout << "S : Spawner" << endl;
    cout << "D : Door" << endl;
    cout << "B : Bomb" << endl;
    cout << "S : Sword" << endl;
    cout << "H : Helth pack" << endl; 
}

void generateRandomMap(Block gameMap[10][20], Player& player, vector<Enemy>& enemies) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            int randomValue = rand() % 100;
            if (i == 0 || i == 9 || j == 0 || j == 19) {
                gameMap[i][j].type = WALL;
            }
            else if (randomValue < 10) {
                gameMap[i][j].type = WALL;
            }
            else if (i == 9 && j == 19) {
                gameMap[i][j].type = DOOR; 
            }
            else if (randomValue < 40) {
                gameMap[i][j].type = GRASS;
            }
            else if (randomValue < 43) {
                gameMap[i][j].type = SWORD; 
            }
            else if (randomValue < 48) {
                gameMap[i][j].type = HEALTH_PACK;
            }
            else if (randomValue < 53) {
                gameMap[i][j].type = BOMB;  
            }
            else {
                gameMap[i][j].type = GRASS;
            }
        }
    }
    int numMonsters = rand() % 3 + 2;
    for (int i = 0; i < numMonsters; i++) {
        Enemy enemy = { rand() % 20, rand() % 10, static_cast<EnemyType>(rand() % 3), 100 };
        enemies.push_back(enemy);
    }
    player.x = 1;
    player.y = 1;
}


void drawMap(Block map[10][20], Player player, std::vector<Enemy> enemies) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (player.x == j && player.y == i) {
                cout << "P";
            }
            else if (map[i][j].type == SWORD) {
                if (player.hasSword) {
                    cout << ".";
                }
                else {
                    cout << "S";
                }
            }
            else {
                bool isEnemyDrawn = false;
                for (const auto& enemy : enemies) {
                    if (enemy.x == j && enemy.y == i && enemy.isAlive()) {
                        switch (enemy.type) {
                        case MELEE:
                            cout << "M";
                            break;
                        case RANGED:
                            cout << "R";
                            break;
                        case TANK:
                            cout << "T";
                            break;
                        }
                        isEnemyDrawn = true;
                        break;
                    }
                }
                if (!isEnemyDrawn) {
                    drawBlock(map[i][j]);
                }
            }
        }
        cout << endl;
    }
}

void handlePickup(Player& player, Block& block) {
    if (block.type == HEALTH_PACK) {
        player.health += 20;
        if (player.health > 100) {
            player.health = 100;
        }
        block.type = GRASS;
    }
}

bool isCollision(Player player, Enemy enemy) {
    return (player.x == enemy.x && player.y == enemy.y);
}



int main() {
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << "        Welcome to the Game! Press SPACE to start." << endl;
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    while (true) {
        char startKey = _getch();
        if (startKey == ' ') {
            break;
        }
    }
    Block gameMap[10][20];
    Player player;
    vector<Enemy> enemies;
    Enemy enemyMelee = { 5, 2, MELEE, 100 };
    Enemy enemyRanged = { 5, 3, RANGED, 100 };
    Enemy enemyTank = { 2, 8, TANK, 100 };
    generateRandomMap(gameMap, player, enemies);
    drawMap(gameMap, player, enemies);
    drawLegend();
    int moveCounter = 0; 
    char input;

    while (true) {
        input = _getch();
        switch (input) {
        case 'w':
            if (gameMap[player.y - 1][player.x].type != WALL) {
                player.y--;
            }
            break;
        case 's':
            if (gameMap[player.y + 1][player.x].type != WALL) {
                player.y++;
            }
            break;
        case 'a':
            if (gameMap[player.y][player.x - 1].type != WALL) {
                player.x--;
            }
            break;
        case 'd':
            if (gameMap[player.y][player.x + 1].type != WALL) {
                player.x++;
            }
            break;
        case 'm': 
            meleeCombat(player, enemyMelee);
            break;
        case 'r':
            rangedCombat(player, enemyRanged);
            break;
        case 't': // Tank combat
            tankCombat(player, enemyTank);
            break;
        case 'e':
            if (gameMap[player.y][player.x].type == HEALTH_PACK)
                handlePickup(player, gameMap[player.y][player.x]);
            break;
        case 'q':
            return 0;
        }
        system("cls");
        cout << "Health: " << player.health << endl;
        cout << "Points: " << player.points << endl;
        moveEnemyMelee(enemyMelee, player, gameMap, 1);
        moveEnemyRanged(enemyRanged, player, gameMap, 3);
        moveEnemyTank(enemyTank, player, gameMap, moveCounter, 4);
        moveCounter++;
        if (gameMap[player.y][player.x].type == SWORD && !player.hasSword) {
            player.equipSword();
            gameMap[player.y][player.x].type = GRASS; 
        }
        if (!player.isAlive()) {
            cout << "Game Over! You have been defeated." << endl;
            return 0;
        }
        else {
            if (gameMap[player.y][player.x].type == BOMB) {
                player.health = 0;
                cout << "Game Over!!" << endl;
                return 0;
            }
            else if ((gameMap[enemyMelee.y][enemyMelee.x].type == BOMB && (abs(player.x - enemyMelee.x) <= 3 && std::abs(player.y - enemyMelee.y) <= 3)) ||
                (gameMap[enemyRanged.y][enemyRanged.x].type == BOMB && (abs(player.x - enemyRanged.x) <= 3 && std::abs(player.y - enemyRanged.y) <= 3)) ||
                    (gameMap[enemyTank.y][enemyTank.x].type == BOMB && (abs(player.x - enemyTank.x) <= 3 && std::abs(player.y - enemyTank.y) <= 3))) {
                player.health = 0;
                cout << "Game Over!" << endl;
                return 0;
            }
            
            if (isCollision(player, enemyMelee) || isCollision(player, enemyRanged) || isCollision(player, enemyTank)) {
                cout << "Game Over!" << endl;
                return 0; // Exits the game
            }

            if (player.x == 19 && player.y == 9) { // Проверка, достиг ли игрок двери
                generateRandomMap(gameMap, player, enemies); // Генерация нового уровня
                drawMap(gameMap, player, enemies);
            }
            drawMap(gameMap, player, { enemyMelee, enemyRanged, enemyTank });
            drawLegend();
        }
    }
    return 0;
}
