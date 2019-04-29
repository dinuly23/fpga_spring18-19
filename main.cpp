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
    int lut_count = 100;
    int lut_connections_count = 10+10+lut_count;
   LUT_Schema dp(lut_count);
   int ans;
   cout << "Started solving" << endl;
   ans = dp.Solve();
   if (ans == -1) {
      cout << "No solution found." << endl;
   } else {
      gene gn = dp.GetGene(ans);

      cout << "The solution is:\n";
      for (int i=0;i<lut_count;++i) {
        cout << "LUT " << i << " has got values: ";
        for (int j=0;j<16;++j) {
            cout << gn.lut_values[i*16+j] << ' ';
        }
        cout << endl;
      }
      for (int i=0;i<(int)number1.size();++i) {
        cout << "Constant number " << i << " from first number with value " << number1[i] << " goes to: ";
        for (int j=0;j<lut_connections_count*2;++j) {
            if (gn.lut_connections[i*lut_connections_count*2+j]==1) {
                if (j%2==0) {
                    cout << j/2 << " ";
                }
            }
        }
        cout << endl;
      }

      for (int i=5;i<5+(int)number2.size();++i) {
        cout << "Constant number " << i-5 << " from second number with value " << number2[i-5] << " goes to: ";
        for (int j=0;j<lut_connections_count*2;++j) {
            if (gn.lut_connections[i*lut_connections_count*2+j]==1) {
                if (j%2==0) {
                    cout << j/2 << " ";
                }
            }
        }
        cout << endl;
      }

      for (int i=10;i<10+lut_count;++i) {
        cout << "LUT " << i-10 << " output goes to: " ;
        for (int j=0;j<lut_connections_count*2;++j) {
            if (gn.lut_connections[i*lut_connections_count*2+j]==1) {
                if (j%2==0) {
                    cout << j/2 << " ";
                }
            }
        }
        cout << endl;
        cout << "LUT " << i-10 << " second output goes to: " ;
        for (int j=0;j<lut_connections_count*2;++j) {
            if (gn.lut_connections[i*lut_connections_count*2+j]==1) {
                if (j%2==1) {
                    cout << j/2 << " ";
                }
            }
        }
        cout << endl;
      }

      cout << "answer is: ";
      for (int i=lut_count+10;i<lut_count+10+10;++i) {
        cout << gn.luts[i].a << ' ';
      }

   }
   return 0;
}
