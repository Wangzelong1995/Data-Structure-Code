#include <iostream>
#include <string>
using namespace std;


void parse(string& input, int& row, int & col){

    string row_str = input.substr(1,input.find(",")-1);
    string col_str = input.substr(input.find(",")+1, input.find(")")-input.find(",")-1);

    cout<<"row is "<<row_str<<" col is "<<col_str<<endl;
    
    // row = stringToInteger(row_str);
    // col = stringToInteger(col_str);
}

int main(){
	cout<<"enter a enter"<<endl;
	// string a;
	int a[] = {1,2,3,4,5};

	cout<<a+2<<endl;

	// cin>>a;
	// int row, col;
	// parse(a,row,col);

	// if (cin.get() == '\n')
	// 	cout<<"haha"<<endl;

	return 0;
}