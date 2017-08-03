// Laser Tag game managing system, Mihai Alexandru Cosmin
#ifndef __LASERTAG__H
#define __LASERTAG__H
#include <list>
struct duelPair
{
    int x, y, n;
    duelPair()
    {
        x = 0;
        y = 0;
        n = 0;
    }
};
struct roundDetails  // game related details
{
    int alive;  // is it alive or not
    bool *vSensors;  // visited array for the sensors that the player
                     // has passed by
    // number of sensors
    // required for the copy-constructor and assignment operator
    int noSensors;
    int lastSensor;  // last visited sensor
    roundDetails()
    {
        this->alive = 2;
        this->vSensors = nullptr;
        this->noSensors = 0;
        this->lastSensor = -1;
    }
    explicit roundDetails(int noSensors)
    {
        this->alive = 2;
        if (noSensors > 0)
        {
            this->vSensors = new bool[noSensors];
            // no sensors visited initially
            memset(this->vSensors, 0, noSensors);
        }
        else
        {
            this->vSensors = nullptr;
        }
        this->lastSensor = -1;
        this->noSensors = noSensors;
    }
    ~roundDetails()
    {
        delete[] this->vSensors;
    }
    // copyconstructor, rule of three
    roundDetails(const roundDetails& other)
    {
        this->alive = other.alive;
        this->lastSensor = other.lastSensor;
        this->noSensors = other.noSensors;
        if (other.noSensors > 0)
        {
            this->vSensors = new bool[other.noSensors];
            memcpy(this->vSensors, other.vSensors, other.noSensors);
        }
        else
        {
            this->vSensors = nullptr;
        }
    }
    // copyassignment, rule of three
    void operator=(const roundDetails& other)
    {
        this->alive = other.alive;
        this->lastSensor = other.lastSensor;
        this->noSensors = other.noSensors;
        if (this->vSensors != nullptr)
        {
            delete[] this->vSensors;
        }
        if (other.noSensors > 0)
        {
            this->vSensors = new bool[other.noSensors];
            memcpy(this->vSensors, other.vSensors, other.noSensors);
        }
        else
        {
            this->vSensors = nullptr;
        }
    }
};
struct championshipDetails  // championship related details
{
    // value used for calculating the top_explorer
    int distance;
    // value for top_shooter. We don't need it at game level,
    // only at championship level
    int killPoints;
    championshipDetails()
    {
        this->distance = 0;
        this->killPoints = 0;
    }
};
class player
{
    char name[50];  // player name
    bool team;  // their team (0 or 1)
    int node;  // associated node in the duels graph
    struct roundDetails round;
    struct championshipDetails champ;
 public:
    player()
    {
        this->name[0] = '\0';
        this->team = 0;
        this->node = -1;
        this->round = roundDetails();
        this->champ = championshipDetails();
    }
    void initializePlayer(const char *name, bool team, int node, int noSensors)
    {
        strcpy(this->name, name);
        this->team = team;
        this->node = node;
        this->round = roundDetails(noSensors);
        this->champ = championshipDetails();
    }
    void updateSensor(int sensor, int** distSensors)
    {
        if (this->round.vSensors[sensor] == 0)
        {
            this->champ.distance += 3;
            this->round.vSensors[sensor] = 1;
        }
        if (this->round.lastSensor != -1)
        {
            this->champ.distance += distSensors[this->round.lastSensor][sensor];
        }
        this->round.lastSensor = sensor;
    }
    void takeDamage()
    {
        --this->round.alive;
    }
    void addPoints(int points)
    {
        this->champ.killPoints += points;
    }
    void resetPlayer()
    {
        this->round.alive = 2;
        this->round.lastSensor = -1;
        memset(this->round.vSensors, 0, this->round.noSensors);
    }
    char* getName()
    {
        return this->name;
    }
    int getNode()
    {
        return this->node;
    }
    bool getTeam()
    {
        return this->team;
    }
    int getAlive()
    {
        return this->round.alive;
    }
    int getScore()
    {
        return this->champ.killPoints;
    }
    int getDistance()
    {
        return this->champ.distance;
    }
};
class LaserTag
{
    std::list<player> *ht;
    // adiacency matrix representing the number of duels between 2 players
    int **duels;
    // adiacency matrix for storing distances between any 2 sensors
    int **distSensors;
    int noSensors;  // number of sensors
    int noPlayers;  // number of players
 public:
    LaserTag()
    {
        this->noSensors = 0;
        this->noPlayers = 0;
        this->ht = nullptr;
        this->duels = nullptr;
        this->distSensors = nullptr;
    }
    unsigned int hashf(char *str)  // hash function
    {
        unsigned int hash = 5381;
        int c;

        while ((c = *str ++) != 0) {
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }
    void newRound()  // new round, reset every player's details
    {
        int i;
        typename std::list<player>::iterator it;
        for (i = 0; i < noPlayers; ++i)
        {
            for (it = ht[i].begin(); it != ht[i].end(); ++it)
            {
                it->resetPlayer();
            }
        }
    }
    void initializeSensors(int noSensors)  // create distSensors matrix
    {
        int i;
        this->noSensors = noSensors;
        this->distSensors = new int*[noSensors];  // adiacency matrix
        for (i = 0; i < noSensors; ++i)
        {
            this->distSensors[i] = new int[noSensors];
        }
    }
    void initializePlayers(int noPlayers)  // create the hashtable
    {
        int i, j;
        this->noPlayers = noPlayers;
        // ht points to noPlayers singly linked lists
        ht = new std::list<player>[noPlayers];
        // adiacency matrix representing the duels between players
        this->duels = new int*[noPlayers];
        for (i = 0; i < noPlayers; ++i)
        {
            this->duels[i] = new int[noPlayers];
            for (j = 0; j < noPlayers; ++j)
            {
                this->duels[i][j] = 0;
            }
        }
    }
    void setSensors(int x, int y, int d)
    {
        distSensors[x][y] = d;
    }
    void addPlayer(char *name, bool team, int node)
    {
        unsigned int h;
        player aux;
        h = hashf(name) % noPlayers;
        aux.initializePlayer(name, team, node, noSensors);
        ht[h].push_back(aux);
    }
    void updateSensor(char *name, int sensor)
    {
        // update for player 'name' the last sensor that he veisited
        unsigned int h;
        typename std::list<player>::iterator it;
        h = hashf(name) % noPlayers;
        for (it = ht[h].begin(); it != ht[h].end(); ++it)
        {
            if (strcmp(it->getName(), name) == 0)
            {
                // update lastSensor and the distance for top_explorer
                it->updateSensor(sensor, distSensors);
                break;
            }
        }
    }
    void updateDuels(char *name1, char *name2)
    {
        unsigned int h;
        h = hashf(name1) % noPlayers;
        typename std::list<player>::iterator it1, it2;
        // search for the struct associated with player 'name1'
        for (it1 = ht[h].begin(); it1 != ht[h].end(); ++it1)
        {
            if (strcmp(it1->getName(), name1) == 0)
            {
                break;
            }
        }
        h = hashf(name2) % noPlayers;
        // search for the struct associated with player 'name2'
        for (it2 = ht[h].begin(); it2 != ht[h].end(); ++it2)
        {
            if (strcmp(it2->getName(), name2) == 0)
            {
                break;
            }
        }
        ++duels[it1->getNode()][it2->getNode()];
        ++duels[it2->getNode()][it1->getNode()];
        it2->takeDamage();
        if (it1->getTeam() == it2->getTeam())
        {
            it1->addPoints(-5);
        }
        else
        {
            it1->addPoints(2);
        }
    }
    int roundWinner()  // check which team won the game
    {
        int i;
        int winner = 1;  // we assume that that the 2nd team won
        typename std::list<player>::iterator it;
        for (i = 0; i < noPlayers && winner == 1; ++i)
        {
            for (it = ht[i].begin(); it != ht[i].end(); ++it)
            {
                // if there is any player from 1st team still alive
                if (it->getTeam() == 0 && it->getAlive() > 0)
                {
                    winner = 0;  // actually the 1st team won
                    break;
                }
            }
        }
        return winner;
    }
    void chanceWinner(int *score1, int *score2)
    {
        int i;
        int maxKillPoints = 0, maxDistance = 1;
        double chance1 = 0, chance2 = 0;
        typename std::list<player>::iterator it;
        for (i = 0; i < noPlayers; ++i)
        {
            for (it = ht[i].begin(); it != ht[i].end(); ++it)
            {
                if (strlen(it->getName()) > 0)  // just in case
                {
                    if (it->getScore() > maxKillPoints)
                    {
                        maxKillPoints = it->getScore();
                    }
                    if (it->getDistance() > maxDistance)
                    {
                        maxDistance = it->getDistance();
                    }
                }
            }
        }
        for (i = 0; i < noPlayers; ++i)
        {
            for (it = ht[i].begin(); it != ht[i].end(); ++it)
            {
                // just in case
                if (strlen(it->getName()) > 0 && it->getAlive() != 0)
                {
                    if (it->getTeam() == 0)
                    {
                        chance1 += (double)it->getScore() / maxKillPoints
                                + (double)it->getDistance() / maxDistance;
                    }
                    else
                    {
                        chance2 += (double)it->getScore() / maxKillPoints
                                + (double)it->getDistance() / maxDistance;
                    }
                }
            }
        }
        if (chance1 >= chance2)
        {
            ++*score1;
        }
        else
        {
            ++*score2;
        }
    }
    int compareAlfa(duelPair a, duelPair b, char **nameof)
    {
        if (strcmp(nameof[a.x], nameof[b.x]) > 0)
        {
            return 0;
        }
        else if (strcmp(nameof[a.x], nameof[b.x]) == 0)
        {
            if (strcmp(nameof[a.y], nameof[b.y]) >= 0)
            {
                return 0;
            }
        }
        return 1;
    }
    void quickSortAlfa(struct duelPair v[], char **nameof, int left, int right)
    {
        int i, j, mid;
        struct duelPair aux, pivot;
        i = left;
        j = right;
        mid = (left + right) / 2;
        pivot = v[mid];
        while (i <= j)
        {
            while (compareAlfa(v[i], pivot, nameof))
            {
                ++i;
            }
            while (compareAlfa(pivot, v[j], nameof))
            {
                --j;
            }
            if (i <= j)
            {
                aux = v[i];
                v[i] = v[j];
                v[j] = aux;
                ++i;
                --j;
            }
        }
        if (i - 1 > left)
        {
            quickSortAlfa(v, nameof, left, i - 1);
        }
        if (i < right)
        {
            quickSortAlfa(v, nameof, i, right);
        }
    }
    void quickSort(struct duelPair v[], int left, int right)
    {
        int i, j, mid, pivot;
        struct duelPair aux;
        i = left;
        j = right;
        mid = (left + right) / 2;
        pivot = v[mid].n;
        while (i <= j)
        {
            while (v[i].n > pivot)
            {
                ++i;
            }
            while (v[j].n < pivot)
            {
                --j;
            }
            if (i <= j)
            {
                aux = v[i];
                v[i] = v[j];
                v[j] = aux;
                ++i;
                --j;
            }
        }
        if (i - 1 > left)
        {
            quickSort(v, left, i - 1);
        }
        if (i < right)
        {
            quickSort(v, i, right);
        }
    }
    void topShooters()
    {
        // array of pointers to players that I will use to sort the
        // players without moving the whole data around
        player **v;
        player* aux;
        typename std::list<player>::iterator it;
        int k, i, pas, ok;
        v = new player*[noPlayers];
        k = -1;
        for (i = 0; i < noPlayers; ++i)
        {
            for (it = ht[i].begin(); it != ht[i].end(); ++it)
            {
                if (strlen(it->getName()) > 0)  // just to be sure
                {
                    ++k;
                    v[k] = &(*it);
                }
            }
        }
        pas = 0;
        ok = 0;
        while (ok == 0)  // sort by score
        {
            ok = 1;
            for (i = 0; i < noPlayers - 1 - pas; ++i)
            {
                if (v[i]->getScore() > v[i + 1]->getScore() ||
                    (v[i]->getScore() == v[i + 1]->getScore() &&
                    strcmp(v[i]->getName(), v[i + 1]->getName()) < 0))
                {
                    ok = 0;
                    aux = v[i];
                    v[i] = v[i + 1];
                    v[i + 1] = aux;
                }
            }
            ++pas;
        }
        printf("I. Top shooters\n");
        for (i = noPlayers - 1; i >= 0 && (noPlayers - i <= 5 ||
            (i < noPlayers - 1 &&
            v[i]->getScore() == v[i + 1]->getScore())); --i)
        {
            printf("%d. %s %dp\n", noPlayers - i,
                                  v[i]->getName(), v[i]->getScore());
        }
        printf("\n");
        delete[] v;
    }
    void topExplorers()
    {
        // array of pointers to players that I will use to sort the
        // players without moving the whole data around
        player** v;
        player* aux;
        typename std::list<player>::iterator it;
        int k, i, pas, ok;
        k = -1;
        v = new player*[noPlayers];
        for (i = 0; i < noPlayers; ++i)
        {
            for (it = ht[i].begin(); it != ht[i].end(); ++it)
            {
                if (strlen(it->getName()) > 0)  // just to be sure
                {
                    ++k;
                    v[k] = &(*it);
                }
            }
        }
        pas = 0;
        ok = 0;
        while (ok == 0)  // sortez dupa distanta
        {
            ok = 1;
            for (i = 0; i < noPlayers - 1 - pas; ++i)
            {
                if (v[i]->getDistance() > v[i + 1]->getDistance() ||
                    (v[i]->getDistance() == v[i + 1]->getDistance() &&
                    strcmp(v[i]->getName(), v[i + 1]->getName()) < 0))
                {
                    ok = 0;
                    aux = v[i];
                    v[i] = v[i + 1];
                    v[i + 1] = aux;
                }
            }
            ++pas;
        }
        printf("II. Top explorers\n");
        for (i = noPlayers - 1; i >= 0 && (noPlayers - i <= 5 ||
            (i < noPlayers - 1 &&
            v[i]->getDistance() == v[i + 1]->getDistance())); --i)
        {
            printf("%d. %s %dp\n", noPlayers - i,
                                  v[i]->getName(), v[i]->getDistance());
        }
        printf("\n");
        delete[] v;
    }
    void topFireExchange()
    {
        // sort the elements from the lower triangular submatrix
        // without the first diagonal
        int i, j, k, no;
        int endp, left, right;
        char **nameof;
        typename std::list<player>::iterator it;
        no = (this->noPlayers - 1) * this->noPlayers / 2;
        struct duelPair *v;
        v = new struct duelPair[no];
        nameof = new char*[noPlayers];
        for (i = 0; i < noPlayers; ++i)
        {
            nameof[i] = new char[50];
        }
        // store the noNode - name mapping
        for (i = 0; i < this->noPlayers; ++i)
        {
            for (it = ht[i].begin(); it != ht[i].end(); ++it)
            {
                if (strlen(it->getName()) > 0)
                {
                    strcpy(nameof[it->getNode()], it->getName());
                }
            }
        }
        k = -1;
        for (i = 0; i < this->noPlayers; ++i)
        {
            for (j = 0; j < i; ++j)
            {
                ++k;
                // store a duel pair alphabetically oredered
                if (strcmp(nameof[i], nameof[j]) < 0)
                {
                    v[k].x = i;
                    v[k].y = j;
                }
                else
                {
                    v[k].x = j;
                    v[k].y = i;
                }
                v[k].n = duels[i][j];
            }
        }
        this->quickSort(v, 0, k);
        endp = 0;
        while (endp < no && (endp < 5 || (endp > 0 &&
                v[endp].n == v[endp - 1].n)))
        {
            ++endp;
        }
        --endp;
        left = 0;
        right = 0;
        while (right < endp)
        {
            // search for all sequences with equal number of duels and
            // sort them alphabetically
            while (v[right].n == v[left].n)
            {
                ++right;
            }
            --right;
            if (right - left > 0)
            {
                this->quickSortAlfa(v, nameof, left, right);
            }
            left = right + 1;
            right = left;
        }
        printf("III. Top fire exchange\n");
        for (i = 0; i < no && (i < 5 ||
            (i > 0 && v[i].n == v[i - 1].n)); ++i)
        {
            printf("%d. %s - %s %d\n", i + 1,
                    nameof[v[i].x], nameof[v[i].y], v[i].n);
        }
        printf("\n");
        delete[] v;
        for (i = 0; i < noPlayers; ++i)
        {
            delete[] nameof[i];
        }
        delete[] nameof;
    }
    void finalScore(int score1, int score2)
    {
        printf("IV. Final score\n");
        printf("%d - %d\n", score1, score2);
    }
    ~LaserTag()
    {
        int i;
        for (i = 0; i < noSensors; ++i)
        {
            if (this->distSensors[i] != nullptr)
            {
                delete[] this->distSensors[i];
            }
        }
        if (this->distSensors != nullptr)
        {
            delete[] this->distSensors;
        }
        for (i = 0; i < noPlayers; ++i)
        {
            if (this->duels[i] != nullptr)
            {
                delete[] this->duels[i];
            }
        }
        if (this->duels != nullptr)
        {
            delete[] this->duels;
        }
        for (i = 0; i < noPlayers; ++i)
        {
            ht[i].clear();
        }
        if (ht != nullptr)
        {
            delete[] ht;
        }
    }
};
#endif
