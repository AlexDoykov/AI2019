#include <iostream>
#include <algorithm>    // std::random_shuffle
#include <random>
#include <set>
#define SIZE 100
using namespace std;

int n;
vector<vector<pair<double, double>>> population;
vector<double> fitnessScores;

double squaredDistance(pair<double, double> p1, pair<double, double> p2){
    return (p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
}

vector<pair<double, double>> createIndividual(vector<pair<double, double>> chromosome){
    vector<pair<double, double>> newIndivid = chromosome;
    random_shuffle(newIndivid.begin(), newIndivid.end());
    return newIndivid;
}

vector<pair<double, double>> createCities(){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_real_distribution<> dist(0, n - 1);
    vector<pair<double, double>> cities;
    cities.resize(n);
    for(int i = 0; i < n; i++) {
        cities[i].first = dist(eng);
        cities[i].second = dist(eng);
    }
    return cities;
}

double fitness(vector<pair<double, double>> individ){
    double fitnessScore = 0;
    for(int i = 0; i < n; i++){
        if(i == n - 1){
            fitnessScore += squaredDistance(individ[i], individ[0]);
        }else{
            fitnessScore += squaredDistance(individ[i], individ[i + 1]);
        }
    }
    return 1/fitnessScore;
}

void createPopulation(vector<pair<double, double>> chromosome){
    for(int i = 0; i < SIZE; i++){
        population[i] = createIndividual(chromosome);
    }
}

double calculatePopulationFitness(){
    double sumFitness = 0;
    for(int i = 0; i < SIZE; i++){
        fitnessScores[i] = fitness(population[i]);
        sumFitness += fitnessScores[i];
    }
    return sumFitness;
}

vector<double> prepareSelection(double sumFitness){
    vector<double> prepare;
    prepare.resize(SIZE);
    double previousProb = 0;
    for(int i = 0; i < SIZE; i++){
        prepare[i] = previousProb + ((double)fitnessScores[i] / sumFitness);
        previousProb = prepare[i];
    }
    return prepare;
}

int selection(vector<double> preparedSelection){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_real_distribution<> dist(0, 1);
    double rnd = dist(eng);
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

 
pair<vector<pair<double, double>>, vector<pair<double, double>>> crossover(vector<pair<double, double>> firstParent, vector<pair<double, double>> secondParent){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_int_distribution<> dist(0, firstParent.size() - 1);
    int rnd1 = dist(eng);
    int rnd2 = dist(eng);

    if(rnd1 > rnd2){
        swap(rnd1, rnd2);
    }

    vector<pair<double, double>> firstOffspring, secondOffspring;
    firstOffspring.resize(firstParent.size());
    secondOffspring.resize(secondParent.size());
    set<double> takenIndexesFirstOffspring, takenIndexesSecondOffspring;
    set<pair<double, double>> takenValuesFirstOffspring, takenValuesSecondOffspring;
    for(int i = rnd1; i <= rnd2; i++){
        firstOffspring[i] = firstParent[i];
        secondOffspring[i] = secondParent[i];
        takenIndexesFirstOffspring.insert(i);
        takenIndexesSecondOffspring.insert(i);
        takenValuesFirstOffspring.insert(firstParent[i]);
        takenValuesSecondOffspring.insert(secondParent[i]);
    }

    int i = rnd1;
    pair<double, double> value = secondParent[i];
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
            pair<double, double> searchValue = firstParent[i];
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
            pair<double, double> searchValue = secondParent[i];
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

vector<pair<double, double>> mutation(vector<pair<double, double>> individual){
    random_device r;
    seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    mt19937 eng(seed);
    uniform_int_distribution<> dist(0, individual.size() - 1);
    int rnd1 = dist(eng);
    int rnd2 = dist(eng);

    pair<double, double> temp = individual[rnd1];
    individual[rnd1] = individual[rnd2];
    individual[rnd2] = individual[rnd1];
    return individual;
}

int main(){
    //cin>>n;
    n = 10;
    fitnessScores.resize(SIZE);
    population.resize(SIZE);
    //vector<pair<double, double>> cities = createCities();
    vector<pair<double, double>> cities = {
        make_pair(59.0110054, 98.3061066),
        make_pair(3.4200027, 7.64650679),
        make_pair(2.30879569, 25.8840599),
        make_pair(11.8902407, 5.5060277),
        make_pair(44.3053741, 40.0689163),
        make_pair(80.4766541, 32.2054329),
        make_pair(20.4433975, 69.24543),
        make_pair(15.7901888, 47.2089005),
        make_pair(98.6976318, 42.9432602),
        make_pair(48.3022346, 75.1088028)
    };
    createPopulation(cities);
    double currentFittest = -1;
    int countTries = 0;
    while(countTries < 6){
        double sumFitness = calculatePopulationFitness();
        if(currentFittest == -1){
            currentFittest = sumFitness;
        }
        if(sumFitness < currentFittest){
            currentFittest = sumFitness;
            countTries = 0;
        }
        cout<<currentFittest<<endl;
        vector<double> prepared = prepareSelection(sumFitness);
        set<int> used;
        vector<vector<pair<double, double>>> wholeOffspring;
        for(int i = 0; i < 20; i++){
            int firstParent;
            do{
                firstParent = selection(prepared);
            }while(used.count(firstParent));
            used.insert(firstParent);
            int secondParent;
            do{
                secondParent = selection(prepared);
            }while(used.count(secondParent));
            used.insert(secondParent);
            pair<vector<pair<double, double>>, vector<pair<double, double>>> offspring;
            offspring = (crossover(population[firstParent], population[secondParent]));
            wholeOffspring.push_back(mutation(offspring.first));
            wholeOffspring.push_back(mutation(offspring.second));
        }

        int j = 0;
        for(int i = 0; i < population.size(); i++){
            if(used.count(i) == 0 && j < wholeOffspring.size()){
                population[i] = wholeOffspring[j];
                j++;
            }
        }
        countTries++;
    }
    cout<<currentFittest<<endl;
    return 0;
}