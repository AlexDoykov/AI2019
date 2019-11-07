#include <iostream>
#include <algorithm>    // std::random_shuffle
#include <random>
#include <set>
using namespace std;

int n;
vector<vector<pair<int, int>>> population;
vector<int> fitnessScores;

int squaredDistance(pair<int, int> p1, pair<int, int> p2){
    return (p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
}

vector<pair<int, int>> createIndividual(vector<pair<int, int>> chromosome){
    vector<pair<int, int>> newIndivid = chromosome;
    random_shuffle(newIndivid.begin(), newIndivid.end());
    return newIndivid;
}

vector<pair<int, int>> createCities(){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_int_distribution<> dist(0, n*n - 1);
    vector<pair<int, int>> cities;
    for(int i = 0; i < n; i++) {
        int rnd = dist(eng);
        int x = rnd % n * 2; // used of % here is just fine
        int y = rnd / n * 2;
        cities[i].first = x;
        cities[i].second = y;
    }
    return cities;
}

int fitness(vector<pair<int, int>> individ){
    int fitnessScore = 0;
    for(int i = 0; i < n; i++){
        if(i == n - 1){
            fitnessScore += squaredDistance(individ[i], individ[0]);
        }else{
            fitnessScore += squaredDistance(individ[i], individ[i + 1]);
        }
    }
    return fitnessScore;
}

void createPopulation(vector<pair<int, int>> chromosome){
    for(int i = 0; i < n % 10; i++){
        population[i] = createIndividual(chromosome);
    }
}

int calculatePopulationFitness(){
    int sumFitness = 0;
    for(int i = 0; i < n % 10; i++){
        fitnessScores[i] = fitness(population[i]);
        sumFitness += fitnessScores[i];
    }
    return sumFitness;
}

vector<double> prepareSelection(int sumFitness){
    vector<double> prepare;
    prepare.resize(n % 10);
    double previousProb = 0;
    for(int i = 0; i < n % 10; i++){
        prepare[i] = previousProb + ((double)fitnessScores[i] / sumFitness);
        previousProb = prepare[i];
    }
    return prepare;
}

int selection(vector<double> preparedSelection){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_int_distribution<> dist(0, 1);
    double rnd = dist(eng);
    int parent;
    bool selected = true;
    for(int j = 0; j < n % 10; j++){
        if( j == n - 1){
            parent = j;
        }else if(preparedSelection[j] < rnd && preparedSelection[j + 1] > rnd){
            parent = j + 1;
        }
    }
    return parent;
}

//pair<vector<int>, vector<int>> 
void crossover(vector<int> firstParent, vector<int> secondParent){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_int_distribution<> dist(0, firstParent.size() - 1);
    int rnd1 = 3;//dist(eng);
    int rnd2 = 7;//dist(eng);

    if(rnd1 > rnd2){
        swap(rnd1, rnd2);
    }

    vector<int> firstOffspring, secondOffspring;
    firstOffspring.resize(firstParent.size());
    secondOffspring.resize(secondParent.size());
    set<int> takenIndexesFirstOffspring, takenIndexesSecondOffspring;
    set<int> takenValuesFirstOffspring, takenValuesSecondOffspring;
    for(int i = rnd1; i <= rnd2; i++){
        firstOffspring[i] = firstParent[i];
        secondOffspring[i] = secondParent[i];
        takenIndexesFirstOffspring.insert(i);
        takenIndexesSecondOffspring.insert(i);
        takenValuesFirstOffspring.insert(firstParent[i]);
        takenValuesSecondOffspring.insert(secondParent[i]);
    }

    int i = rnd1;
    int value = secondParent[i];
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
            int searchValue = firstParent[i];
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
    while(i <= rnd2 || !stop){
        int j = rnd1;
        for(; j <= rnd2; j++){
            if(value == secondOffspring[j]){
                break;
            }
        }
        if(j == rnd2 + 1){
            value = secondParent[i];
            int j = 0;
            for(; j <= firstParent.size(); j++){
                if(value == firstParent[j]){
                    break;
                }
            }
            if(j >= rnd1 && j <= rnd2){
                i = j;
            }else{
                secondOffspring[j] = value;
                takenIndexesSecondOffspring.insert(j);
                takenValuesSecondOffspring.insert(value);
            }
        }else{
            i++;
            value = firstParent[i];
        }
    }

    for(int i = 0; i < firstParent.size(); i++){
        if(takenIndexesSecondOffspring.count(i) == 0){
            for(int j = 0; j < firstParent.size(); j++){
                if(takenValuesFirstOffspring.count(firstParent[j])){
                    secondOffspring[i] = firstParent[j];
                    takenIndexesSecondOffspring.insert(i);
                    takenValuesSecondOffspring.insert(firstParent[j]);
                }
            }
        }
    }

    cout<<"FIRST OFFSPRING:\n";
    for(int i = 0; i < firstOffspring.size(); i++){
        cout<<firstOffspring[i]<<" ";
    }
    cout<<endl;

    cout<<"SECOND OFFSPRING:\n";
    for(int i = 0; i < firstOffspring.size(); i++){
        cout<<secondOffspring[i]<<" ";
    }
    cout<<endl;
}

int main(){
    // cin>>n;
    // fitnessScores.resize(n % 10);
    // population.resize(n % 10);
    // vector<pair<int, int>> cities = createCities();
    // createPopulation(cities);
    // int sumFitness = calculatePopulationFitness();
    // vector<double> prepared = prepareSelection(sumFitness);
    // set<int> used;
    // for(int i = 0; i < n % 20; i++){
    //     int firstParent;
    //     do{
    //         firstParent = selection(prepared);
    //     }while(used.count(firstParent));
    //     used.insert(firstParent);
    //     int secondParent;
    //     do{
    //         secondParent = selection(prepared);
    //     }while(used.count(secondParent));
    //     used.insert(secondParent);
    //     crossover(firstParent, secondParent);

    // }

    crossover(vector<int>{8, 4, 7, 3, 6, 2, 5, 1, 9, 0}, vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    return 0;
}