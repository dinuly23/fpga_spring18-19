#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <algorithm>

#define MAXPOP	100
#define MAXTESTS	100

using namespace std;

struct LUT {
	bool a,b,c,d;
	//vector<pair<int, int>> type; // 0 - выход , 1- константа, 2 - номер переменной
	vector<bool> input;
	vector<bool> output;
	LUT(vector<bool> _output):output(_output) {
		a=0;
		b=0;
		c=0;
		d=0;
	}
	LUT(const LUT& _lut) {
		this->a=_lut.a;
		this->b=_lut.b;
		this->c=_lut.c;
		this->d=_lut.d;
		this->output=_lut.output;
	}
	void seta(bool _a) {
		this->a=_a;
	}
	void setb(bool _b) {
		this->b=_b;
	}
	void setc(bool _c) {
		this->c=_c;
	}
	void setd(bool _d) {
		this->d=_d;
	}
	pair<bool,bool> get() {
	    for (int i=0;i<(int)input.size();++i) {
            if (i==0) {
                a=input[i];
            }
            else if (i==1) {
                b=input[i];
            }
            else if (i==2) {
                c=input[i];
            }
            else if (i==3) {
                d=input[i];
            }
	    }
		return make_pair(output[output.size()-((int)d*8+(int)c*4+(int)b*2+(int)a+1)],output[output.size()-((int)0*8+(int)c*4+(int)b*2+(int)a+1)]);//Первый элемент - output, второй - carry_out
	}
};

struct gene {
	vector<bool> lut_values;
	vector<bool> lut_connections;
	vector<LUT> luts;
	int fitness;
	double likelihood;
	int lut_count;

	gene(int _lut_count):lut_count(_lut_count) {
		vector<bool> vec = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        for (int i=0;i<lut_count*16;++i) {
			lut_values.push_back(0);
		}
		int lut_connections_count = 10+10+lut_count;
		for (int i=0;i<lut_connections_count*lut_connections_count*2;++i) {
			lut_connections.push_back(0);
		}
        for (int i=0;i<lut_connections_count;++i) {
			luts.push_back(LUT(vec));
		}
	}

	// Test for equality.
	bool operator==(gene gn) {
		for (int i=0;i<lut_count*16;i++) {
			if (gn.lut_values[i] != lut_values[i]) return false;
		}
		int lut_connections_count = 10+10+lut_count;
        for (int i=0;i<lut_connections_count*lut_connections_count*2;i++) {
			if (gn.lut_connections[i] != lut_connections[i]) return false;
		}
		return true;
	}

	gene operator=(gene gn) {
	    this->lut_values=gn.lut_values;
	    this->lut_connections=gn.lut_connections;
	    this->luts=gn.luts;
	    this->fitness=gn.fitness;
	    this->likelihood=gn.likelihood;
	    this->lut_count=gn.lut_count;
	    return (*this);
	}
};

class LUT_Schema {
	public:
		LUT_Schema(int);// Constructor with coefficients for a,b,c,d.
		int Solve();// Solve the equation.

		// Returns a given gene.
		gene GetGene(int i) { return population[i];}

	protected:
		int lut_count;
		vector<gene> population;// Population.

		int Fitness(gene &,int,int);// Fitness function.
		void GenerateLikelihoods();	// Generate likelihoods.
		double MultInv();// Creates the multiplicative inverse.
		int CreateFitnesses();
		void CreateNewPopulation();
		int GetIndex(double val);

		gene Breed(int p1, int p2);

};

LUT_Schema::LUT_Schema(int _lut_count) : lut_count(_lut_count) {
    for (int i=0;i<MAXPOP;++i) {
        population.push_back(gene(_lut_count));
    }
}

int LUT_Schema::Solve() {
	int fitness = -1;

	// Generate initial population.
	srand((unsigned)time(NULL));

	int lut_connections_count = 10+10+lut_count;
	for(int i=0;i<MAXPOP;i++) {// Fill the population with numbers between
		cout << "Gene " << i << " is being created" << endl;
		for (int j=0;j<lut_count*16;j++) {// 0 and the result.
			population[i].lut_values[j] = rand() % 2;
		}
		cout << "LUT values are set" << endl;
		for (int j=10;j<lut_connections_count-10;++j) {
            vector<bool> v;
            for (int k=0;k<16;++k) {
                v.push_back(population[i].lut_values[(j-10)*16+k]);
            }
            population[i].luts[j].output = v;
		}
		cout << "luts are created" << endl;
		for (int j=0;j<lut_connections_count*lut_connections_count*2;j++) {// 0 and the result.
            if (j/(lut_connections_count*2)<(j%(lut_connections_count*2))/2
                && (j%(lut_connections_count*2))/2>=10)
                population[i].lut_connections[j] = rand() % 2;
            else
                population[i].lut_connections[j] = 0;
		}
		cout << "LUTs connections are set" << endl;
	}
	cout << "Generated starting population" << endl;

	if ((fitness = CreateFitnesses())) {
		return fitness;
	}
	cout << "Counted fitness " << fitness << endl;

	int iterations = 0;// Keep record of the iterations.
	while (fitness != 0 || iterations < 100) {// Repeat until solution found, or over 50 iterations.
        cout << "Started iteration " << iterations << endl;
		GenerateLikelihoods();// Create the likelihoods.
        cout << "Generated Likelihoods iteration " << iterations << endl;
		CreateNewPopulation();
        cout << "Created Population iteration " << iterations << endl;
		if ((fitness = CreateFitnesses())) {
			return fitness;
		}
        cout << "Counted fitness iteration " << iterations << ": " << fitness << endl;
		iterations++;
	}

	return -1;
}

int LUT_Schema::Fitness(gene &gn,int num1,int num2) {
	vector<bool> number1,number2;
	int _num1,_num2;
	_num1=num1;
	_num2=num2;
	for (int i=0;i<5;++i) {
		number1.push_back(_num1%2);
		_num1/=2;
		number2.push_back(_num2%2);
		_num2/=2;
	}
	reverse(number1.begin(),number1.end());
	reverse(number2.begin(),number2.end());
    queue<int> q;
    int lut_connections_count = 10+10+lut_count;
    int used[lut_connections_count];
    for (int i=0;i<lut_connections_count;++i) {
        used[i]=0;
    }
    for (int i=0;i<(lut_connections_count*2)*10; ++i) {
        if (gn.lut_connections[i]) {
                //gn.luts[(i%(lut_connections_count*2))/2].input.push_back(i/(lut_connections_count*2));
                if (i/(lut_connections_count*2)<5)
                    gn.luts[(i%(lut_connections_count*2))/2].input.push_back(number1[i/(lut_connections_count*2)]);
                else {
                    gn.luts[(i%(lut_connections_count*2))/2].input.push_back(number2[i/(lut_connections_count*2)-5]);
                }
                if (!used[(i%(lut_connections_count*2))/2]) {
                    used[(i%(lut_connections_count*2))/2]=1;
                    q.push((i%(lut_connections_count*2))/2);
                }
        }
    }
	while (!q.empty()) {
        int cur_lut = q.front();
        q.pop();
        used[cur_lut]=1;
        pair<int,int> ans = gn.luts[cur_lut].get();
        for (int i=cur_lut*lut_connections_count*2;i<(cur_lut+1)*lut_connections_count*2;++i) {
            if (gn.lut_connections[i]) {
                //gn.luts[(i%(lut_connections_count*2))/2].input.push_back(i/(lut_connections_count*2));
                if ((i%(lut_connections_count*2))%2==0)
                    gn.luts[(i%(lut_connections_count*2))/2].input.push_back(ans.first);
                else {
                    gn.luts[(i%(lut_connections_count*2))/2].input.push_back(ans.second);
                }
                if (!used[(i%(lut_connections_count*2))/2]) {
                    used[(i%(lut_connections_count*2))/2]=1;
                    q.push((i%(lut_connections_count*2))/2);
                }
            }
        }
	}
	int total=0;
    for (int i=10+lut_count;i<lut_connections_count;++i) {
        total+=(int)(gn.luts[i].a)*(1<<(lut_connections_count-i-1));
    }
    //cout << "total in fitness is " << total << ", num1 is " << num1 << ", num2 is " << num2 << endl;
	return abs(total - num1*num2);
}

int LUT_Schema::CreateFitnesses() {
	double avgfit = 0;
	int fitness = 0;
	int num1,num2;
	for(int i=0;i<MAXPOP;i++) {
        cout << "Counting fitness for " << i << endl;
        fitness=0;
        for (int j=0;j<MAXTESTS;++j) {
			num1=rand()%(1<<5);
			num2=rand()%(1<<5);
			fitness += Fitness(population[i],num1,num2);
		}
		population[i].fitness=fitness;
        cout << "Counted fitness for " << i << ": " << fitness << endl;
		avgfit += fitness;
		if (fitness == 0) {
			return i;
		}
	}

	return 0;
}

double LUT_Schema::MultInv() {
	double sum = 0;
	cout << "Counting MultInv" << endl;
	for(int i=0;i<MAXPOP;i++) {
		cout << population[i].fitness << ' ';
		sum += 1/((double)population[i].fitness);
	}
	cout << endl;
	return sum;
}

void LUT_Schema::GenerateLikelihoods() {
	double multinv = MultInv();

	double last = 0;
	cout << "Likelihoods are like this: ";
	for(int i=0;i<MAXPOP;i++) {
		population[i].likelihood = last = last + ((1/((double)population[i].fitness) / multinv) * 100);
		cout << population[i].likelihood << ' ';
	}
	cout << endl;
}

int LUT_Schema::GetIndex(double val) {
	double last = 0;
	for(int i=0;i<MAXPOP;i++) {
		if (last <= val && val <= population[i].likelihood) return i;
		else last = population[i].likelihood;
	}
	return rand()%MAXPOP;
}

gene LUT_Schema::Breed(int p1, int p2) {
    int lut_connections_count = 10+10+lut_count;

	int crossover_lut_values = rand() % (lut_count*16-1)+1;// Create the crossover point (not first).
	int crossover_lut_connections = rand() % (lut_connections_count*lut_connections_count*2*-1)+1;// Create the crossover point (not first).
	int first = rand() % 100;// Which parent comes first?

	gene child = population[p1];// Child is all first parent initially.

	int initial_lut_values = 0, final_lut_values = (lut_count*16-1);// The crossover boundaries.
	int initial_lut_connections = 0, final_lut_connections = (lut_connections_count*lut_connections_count*2*-1);// The crossover boundaries.
	if (first < 50) {
        initial_lut_values = crossover_lut_values;
        initial_lut_connections = crossover_lut_connections;
    } // If first parent first. start from crossover.
	else {
        final_lut_values = crossover_lut_values+1;// Else end at crossover
        final_lut_connections = crossover_lut_connections + 1;
	}

	for(int i=initial_lut_values;i<final_lut_values;i++) {// Crossover!
		child.lut_values[i] = population[p2].lut_values[i];
		if (rand() % 101 < 5) child.lut_values[i] = rand() % 2;
	}

	for(int i=initial_lut_connections;i<final_lut_connections;i++) {// Crossover!
		child.lut_connections[i] = population[p2].lut_connections[i];
		if (i/(lut_connections_count*2)<(i%(lut_connections_count*2))/2
            && (i%(lut_connections_count*2))/2>=10) {
            if (rand() % 101 < 5) child.lut_connections[i] = rand() % 2;
		}
	}

	return child;// Return the kid...
}

void LUT_Schema::CreateNewPopulation() {
	vector<gene> temppop;
	//cout << "temp genes are created" << endl;
	for(int i=0;i<MAXPOP;i++) {
		int parent1 = 0, parent2 = 0, iterations = 0;
		while(parent1 == parent2 || population[parent1] == population[parent2]) {
			//cout << "getting first parent" << endl;
			parent1 = GetIndex((double)(rand() % 101));
			//cout << "getting second parent" << endl;
			parent2 = GetIndex((double)(rand() % 101));
			//cout << parent1 << ' ' << parent2 << endl;
			/*if (population[parent1] == population[parent2]) {
				cout << "They are equal" << endl;
			}
			else {
				cout << "They are not equal" << endl;
			}*/
			if (++iterations > 25) break;
		}
        //cout << "Right before breeding " << i << endl;
		temppop.push_back(Breed(parent1, parent2));// Create a child.
        //cout << "Right after breeding " << i << endl;
	}
    //cout << "Updating population" << endl;
	for(int i=0;i<MAXPOP;i++) population[i] = temppop[i];
    //cout << "Updating population ended" << endl;
}
