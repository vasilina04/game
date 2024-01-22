#include <iostream>
#include <conio.h>

using namespace std;

enum BlockType {
    WALL,
    GRASS,
    SPAWNER,
    DOOR
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
    int rangedDamage = 10;
    int tankDamage = 20;

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
        player.addPoints(10); // Add points for victory
        std::cout << "Player won. Points earned: " << player.points << std::endl;
        // Handle player's victory - например, удалить противника с поля
        enemy.health = 0; // "Убить" противника
    }
    else {
        std::cout << "Player lost. Points earned: " << player.points << std::endl;
        // Handle player's defeat
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
        player.addPoints(30); // Add points for victory
        // Handle player's victory
    }
    else {
        std::cout << "Player lost. Points earned: " << player.points << std::endl;
        // Handle player's defeat
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
        player.addPoints(50); // Add points for victory
        // Handle player's victory
    }
    else {
        std::cout << "Player lost. Points earned: " << player.points << std::endl;
        // Handle player's defeat
    }
}
void moveEnemy(Enemy& enemy, Block gameMap[10][20]) {
    int direction = rand() % 4; // Случайное определение направления движения: 0 - вверх, 1 - вниз, 2 - влево, 3 - вправо
    int newX = enemy.x;
    int newY = enemy.y;

    switch (direction) {
    case 0: // Вверх
        newY--;
        break;
    case 1: // Вниз
        newY++;
        break;
    case 2: // Влево
        newX--;
        break;
    case 3: // Вправо
        newX++;
        break;
    }

    if (gameMap[newY][newX].type != WALL) {
        enemy.x = newX;
        enemy.y = newY;
    }
}

void moveEnemyMelee(Enemy& enemy, Player& player, Block gameMap[10][20], int radius) {
    if (std::abs(player.x - enemy.x) <= radius && std::abs(player.y - enemy.y) <= radius) {
        player.takeDamage(enemy.meleeDamage);
    }
    else {
        int direction = rand() % 4; // Randomly choose a direction: 0 - up, 1 - down, 2 - left, 3 - right
        int newX = enemy.x;
        int newY = enemy.y;

        switch (direction) {
        case 0: // Up
            newY--;
            break;
        case 1: // Down
            newY++;
            break;
        case 2: // Left
            newX--;
            break;
        case 3: // Right
            newX++;
            break;
        }

        if (gameMap[newY][newX].type != WALL) {
            enemy.x = newX;
            enemy.y = newY;
        }
    }
}

void moveEnemyRanged(Enemy& enemy, Player& player, Block gameMap[10][20], int radius) {
    if (std::abs(player.x - enemy.x) <= radius && std::abs(player.y - enemy.y) <= radius) {
        player.takeDamage(enemy.rangedDamage);
    }
    else {
        int direction = rand() % 4; // Randomly choose a direction
        int newX = enemy.x;
        int newY = enemy.y;

        switch (direction) {
        case 0: // Up
            newY--;
            break;
        case 1: // Down
            newY++;
            break;
        case 2: // Left
            newX--;
            break;
        case 3: // Right
            newX++;
            break;
        }

        if (gameMap[newY][newX].type != WALL) {
            enemy.x = newX;
            enemy.y = newY;
        }
    }
}

void moveEnemyTank(Enemy& enemy, Player& player, Block gameMap[10][20], int& moveCounter, int radius) {
    if (moveCounter % 2 == 0) {
        if (std::abs(player.x - enemy.x) <= radius && std::abs(player.y - enemy.y) <= radius) {
            player.takeDamage(enemy.tankDamage);
        }
        else {
            int direction = rand() % 4; // Randomly choose a direction: 0 - up, 1 - down, 2 - left, 3 - right
            int newX = enemy.x;
            int newY = enemy.y;

            switch (direction) {
            case 0: // Up
                newY--;
                break;
            case 1: // Down
                newY++;
                break;
            case 2: // Left
                newX--;
                break;
            case 3: // Right
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
    case SPAWNER:
        cout << "S";
        break;
    case DOOR:
        cout << "D";
        break;
    }
}

void drawMap(Block map[10][20], Player player, Enemy enemy) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (player.x == j && player.y == i) {
                cout << "P";
            }
            else if (enemy.x == j && enemy.y == i) {
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
            }
            else {
                drawBlock(map[i][j]);
            }
        }
        cout << endl;
    }
}

bool isCollision(Player player, Enemy enemy) {
    return (player.x == enemy.x && player.y == enemy.y);
}

int main() {
    Block gameMap[10][20] = {
       {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
       {WALL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, WALL, WALL, WALL, WALL, WALL, WALL},
       {WALL, SPAWNER, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, WALL, WALL, WALL, WALL, WALL, WALL},
       {WALL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, WALL},
       {WALL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, WALL, GRASS, WALL, WALL, GRASS, WALL},
       {WALL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, WALL, GRASS, WALL, WALL, GRASS, WALL},
       {WALL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, WALL, GRASS, WALL, WALL, GRASS, WALL},
       {WALL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, WALL, WALL, WALL, WALL, GRASS, WALL},
       {WALL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, DOOR, GRASS, GRASS, GRASS, GRASS, WALL},
       {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL}
    };

    Player player = { 1, 1, 100 };
    Enemy enemyMelee = { 5, 2, MELEE, 100 };
    Enemy enemyRanged = { 5, 3, RANGED, 100 };
    Enemy enemyTank = { 2, 8, TANK, 100 };

    drawMap(gameMap, player, enemyMelee);//Draws the map with players and enemies

    char input;
    while (true) {
        input = _getch();
        switch (input) {
        case 'w': // Вверх
            if (gameMap[player.y - 1][player.x].type != WALL) {
                player.y--;
            }
            break;
        case 's': // Вниз
            if (gameMap[player.y + 1][player.x].type != WALL) {
                player.y++;
            }
            break;
        case 'a': // Влево
            if (gameMap[player.y][player.x - 1].type != WALL) {
                player.x--;
            }
            break;
        case 'd':// Вправо
            if (gameMap[player.y][player.x + 1].type != WALL) {
                player.x++;
            }
            break;
        case 'm': // Melee combat
            meleeCombat(player, enemyMelee);
            break;
        case 'r': // Ranged combat
            rangedCombat(player, enemyRanged);
            break;
        case 't': // Tank combat
            tankCombat(player, enemyTank);
            break;
        case 'q': // Quit the game
            return 0;
        }
        system("cls");
        cout << "Health: " << player.health << endl;
        cout << "Points: " << player.points << endl;

        // Проверка условий победы/поражения и отрисовка карты
        if (!player.isAlive()) {
            cout << "Game Over! You have been defeated." << endl;
            return 0;
        }
        else {
            drawMap(gameMap, player, enemyMelee);
            if (isCollision(player, enemyMelee) || isCollision(player, enemyRanged) || isCollision(player, enemyTank)) {
                cout << "You lose!" << endl;
                return 0; // Exits the game
            }
        }
    }
    return 0;
}