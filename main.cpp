#include <iostream>
#include "lut_schema.h"

string s1,s2;
vector<bool> number1,number2;
int num1,num2;

int main() {
    /*
01101
10001
    */
    /*freopen("1.txt","r",stdin);
    cin >> s1 >> s2;
    for (int i=0;i<(int)s1.size();++i) {
        number1.push_back(s1[i]-'0');
        cout << number1[i] << ' ';
        num1+=number1[i]*(1<<(s1.size()-i-1));
    }
    cout << endl;
    for (int i=0;i<(int)s2.size();++i) {
        number2.push_back(s2[i]-'0');
        num2+=number2[i]*(1<<(s2.size()-i-1));
        cout << number2[i] << ' ';
    }
    cout << endl;
    cout << num1 << ' ' << num2 << endl;
	*/
	int bytes_count=4;
	int left_border=50;
	int right_border=120;
	freopen("out.txt","w",stdout);
   LUT_Schema dp(bytes_count,left_border,right_border);
   cout << "Started solving" << endl;
   dp.Solve();
    int lut_count = dp.best_lut_count;
    int lut_connections_count = bytes_count*2+bytes_count*2+lut_count;
   cout << "Best fitness was achieved with " << dp.best_lut_count << " luts" << endl;
   cout << "Best fitness was: " << dp.best_fitness << endl;
  gene gn = dp.best_gene;

  cout << "The best gene is:\n";
  for (int i=0;i<lut_count;++i) {
    cout << "LUT " << i << " has got values: ";
    for (int j=0;j<16;++j) {
        cout << gn.lut_values[i*16+j] << ' ';
    }
    cout << endl;
  }
  for (int i=0;i<bytes_count;++i) {
    cout << "Constant number " << i << " from first number goes to: ";
    for (int j=0;j<lut_connections_count*2;++j) {
        if (gn.lut_connections[i*lut_connections_count*2+j]==1) {
            if (j%2==0) {
                cout << j/2 << " ";
            }
        }
    }
    cout << endl;
  }

  for (int i=bytes_count;i<bytes_count*2;++i) {
    cout << "Constant number " << i-bytes_count << " from second number goes to: ";
    for (int j=0;j<lut_connections_count*2;++j) {
        if (gn.lut_connections[i*lut_connections_count*2+j]==1) {
            if (j%2==0) {
                cout << j/2 << " ";
            }
        }
    }
    cout << endl;
  }

  for (int i=bytes_count*2;i<bytes_count*2+lut_count;++i) {
    cout << "LUT " << i-bytes_count*2 << " output goes to: " ;
    for (int j=0;j<lut_connections_count*2;++j) {
        if (gn.lut_connections[i*lut_connections_count*2+j]==1) {
            if (j%2==0) {
                cout << j/2 << " ";
            }
        }
    }
    cout << endl;
    cout << "LUT " << i-bytes_count*2 << " second output goes to: " ;
    for (int j=0;j<lut_connections_count*2;++j) {
        if (gn.lut_connections[i*lut_connections_count*2+j]==1) {
            if (j%2==1) {
                cout << j/2 << " ";
            }
        }
    }
    cout << endl;
  }

  /*cout << "answer is: ";
  for (int i=lut_count+10;i<lut_count+10+10;++i) {
    cout << gn.luts[i].a << ' ';
  }*/
   return 0;
}
