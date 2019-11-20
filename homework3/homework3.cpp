#include <iostream>
#include <algorithm>    // std::random_shuffle
#include <random>
#include <set>
#define SIZE 100
using namespace std;

int n;
vector<vector<pair<float, float>>> population;
vector<float> fitnessScores;

float squaredDistance(pair<float, float> p1, pair<float, float> p2){
    return (p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
}

vector<pair<float, float>> createIndividual(vector<pair<float, float>> chromosome){
    vector<pair<float, float>> newIndivid = chromosome;
    random_shuffle(newIndivid.begin(), newIndivid.end());
    return newIndivid;
}

vector<pair<float, float>> createCities(){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_real_distribution<> dist(0, n - 1);
    vector<pair<float, float>> cities;
    cities.resize(n);
    for(int i = 0; i < n; i++) {
        cities[i].first = dist(eng);
        cities[i].second = dist(eng);
    }
    return cities;
}

float fitness(vector<pair<float, float>> individ){
    float fitnessScore = 0;
    for(int i = 0; i < n; i++){
        if(i == n - 1){
            fitnessScore += squaredDistance(individ[i], individ[0]);
        }else{
            fitnessScore += squaredDistance(individ[i], individ[i + 1]);
        }
    }
    return fitnessScore;
}

void createPopulation(vector<pair<float, float>> chromosome){
    for(int i = 0; i < SIZE; i++){
        population[i] = createIndividual(chromosome);
    }
}

pair<float, float> calculatePopulationFitness(){
    float fitnessScore = fitness(population[0]);
    fitnessScores[0] = 1/fitnessScore;
    float sumFitness = fitnessScores[0];
    float minFitness = fitnessScore;
    for(int i = 1; i < SIZE; i++){
        fitnessScore = fitness(population[i]);
        fitnessScores[i] = 1/fitnessScore;
        sumFitness += fitnessScores[i];
        if(fitnessScore < minFitness){
            minFitness = fitnessScore;
        }
    }
    return make_pair(sumFitness, minFitness);
}

vector<float> prepareSelection(float sumFitness){
    vector<float> prepare;
    prepare.resize(SIZE);
    float previousProb = 0;
    for(int i = 0; i < SIZE; i++){
        prepare[i] = previousProb + ((float)fitnessScores[i] / sumFitness);
        previousProb = prepare[i];
    }
    return prepare;
}

int selection(vector<float> preparedSelection){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_real_distribution<> dist(0, 1);
    float rnd = dist(eng);
    int parent;
    for(int j = 0; j < SIZE; j++){
        if( j == SIZE - 1){
            parent = j;
        }else if(preparedSelection[j] <= rnd && preparedSelection[j + 1] >= rnd){
            return parent = j + 1;
        }
    }
    return parent;
}

 
pair<vector<pair<float, float>>, vector<pair<float, float>>> crossover(vector<pair<float, float>> firstParent, vector<pair<float, float>> secondParent){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_int_distribution<> dist(0, firstParent.size() - 1);
    int rnd1 = dist(eng);
    int rnd2 = dist(eng);

    if(rnd1 > rnd2){
        swap(rnd1, rnd2);
    }

    vector<pair<float, float>> firstOffspring, secondOffspring;
    firstOffspring.resize(firstParent.size());
    secondOffspring.resize(secondParent.size());
    set<float> takenIndexesFirstOffspring, takenIndexesSecondOffspring;
    set<pair<float, float>> takenValuesFirstOffspring, takenValuesSecondOffspring;
    for(int i = rnd1; i <= rnd2; i++){
        firstOffspring[i] = firstParent[i];
        secondOffspring[i] = secondParent[i];
        takenIndexesFirstOffspring.insert(i);
        takenIndexesSecondOffspring.insert(i);
        takenValuesFirstOffspring.insert(firstParent[i]);
        takenValuesSecondOffspring.insert(secondParent[i]);
    }

    int i = rnd1;
    pair<float, float> value = secondParent[i];
    bool stop = false;
    while(i <= rnd2){
        int j = rnd1;
        //check if the value has been copied to the child
        for(; j <= rnd2; j++){
            if(value == firstOffspring[j]){
                break;
            }
        }
        //if not
        if(j == rnd2 + 1){
            pair<float, float> searchValue = firstParent[i];
            bool exit = true;
            while(exit){
                int j = 0;
                for(; j <= secondParent.size(); j++){
                    if(searchValue == secondParent[j]){
                        break;
                    }
                }
                if(j >= rnd1 && j <= rnd2){
                    searchValue = firstParent[j];
                }else{
                    firstOffspring[j] = value;
                    takenIndexesFirstOffspring.insert(j);
                    takenValuesFirstOffspring.insert(value);
                    exit = false;
                }
            }
        }
        //if it has been copied
        i++;
        value = secondParent[i];
    }

    for(int i = 0; i < secondParent.size(); i++){
        if(takenValuesFirstOffspring.count(secondParent[i]) == 0){
            for(int j = 0; j < secondParent.size(); j++){
                if(takenIndexesFirstOffspring.count(j) == 0){
                    firstOffspring[j] = secondParent[i];
                    takenIndexesFirstOffspring.insert(j);
                    takenValuesFirstOffspring.insert(secondParent[i]);
                    break;
                }
            }
        }
    }

    i = rnd1;
    value = firstParent[i];
    stop = false;
    while(i <= rnd2){
        int j = rnd1;
        //check if the value has been copied to the child
        for(; j <= rnd2; j++){
            if(value == secondOffspring[j]){
                break;
            }
        }
        //if not
        if(j == rnd2 + 1){
            pair<float, float> searchValue = secondParent[i];
            bool exit = true;
            while(exit){
                int j = 0;
                for(; j <= firstParent.size(); j++){
                    if(searchValue == firstParent[j]){
                        break;
                    }
                }
                if(j >= rnd1 && j <= rnd2){
                    searchValue = secondParent[j];
                }else{
                    secondOffspring[j] = value;
                    takenIndexesSecondOffspring.insert(j);
                    takenValuesSecondOffspring.insert(value);
                    exit = false;
                }
            }
        }
        i++;
        value = firstParent[i];
    }

    for(int i = 0; i < firstParent.size(); i++){
        if(takenValuesSecondOffspring.count(firstParent[i]) == 0){
            for(int j = 0; j < firstParent.size(); j++){
                if(takenIndexesSecondOffspring.count(j) == 0){
                    secondOffspring[j] = firstParent[i];
                    takenIndexesSecondOffspring.insert(j);
                    takenValuesSecondOffspring.insert(firstParent[i]);
                    break;
                }
            }
        }
    }

    return make_pair(firstOffspring, secondOffspring);
}

vector<pair<float, float>> mutate(vector<pair<float, float>> individual){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_int_distribution<> dist(0, individual.size() - 1);
    int rnd1 = dist(eng);
    int rnd2 = dist(eng);

    pair<float, float> temp = individual[rnd1];
    individual[rnd1] = individual[rnd2];
    individual[rnd2] = temp;
    return individual;
}

void addChild(vector<pair<float, float>> child, vector<float> normalFitnesses){
    int smallestElementIndex = max_element(normalFitnesses.begin(), normalFitnesses.end()) - normalFitnesses.begin();
    population[smallestElementIndex] = child;
}

int main(){
    //cin>>n;
    n = 10;
    fitnessScores.resize(SIZE);
    population.resize(SIZE);
    //vector<pair<float, float>> cities = createCities();
    vector<pair<float, float>> cities = {
        make_pair(59.011, 98.3061),
        make_pair(3.42003, 7.64651),
        make_pair(2.3088, 25.8841),
        make_pair(11.8902, 5.50603),
        make_pair(44.3054, 40.0689),
        make_pair(80.4767, 32.2054),
        make_pair(20.4434, 69.2454),
        make_pair(15.7902, 47.2089),
        make_pair(98.6976, 42.9433),
        make_pair(48.3022, 75.1088)
    };
    createPopulation(cities);
    float currentFittest = -1;
    int countTries = 0;
    while(countTries < 260){
        pair<float, float> sumAndMaxFitness = calculatePopulationFitness();
        float sumFitness = sumAndMaxFitness.first;
        float maxFitness = sumAndMaxFitness.second;
        if(currentFittest == -1){
            currentFittest = maxFitness;
        }
        if(maxFitness < currentFittest){
            currentFittest = maxFitness;
            countTries = 0;
        }
        vector<float> prepared = prepareSelection(sumFitness);
        set<int> used;
        for(int i = 0; i < 20; i++){
            int firstParent;
            firstParent = selection(prepared);
            used.insert(firstParent);
            int secondParent;
            secondParent = selection(prepared);
            used.insert(secondParent);
            pair<vector<pair<float, float>>, vector<pair<float, float>>> offspring;
            offspring = (crossover(population[firstParent], population[secondParent]));
            if(fitness(offspring.first) > currentFittest){
                offspring.first = mutate(offspring.first);
            }
            addChild(offspring.first, prepared);
            if(fitness(offspring.first) > currentFittest){
                offspring.first = mutate(offspring.first);
            }
            addChild(offspring.second, prepared);
        }
        countTries++;
    }
    cout<<sqrt(currentFittest)<<endl;
    return 0;
}