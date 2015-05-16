#include<iostream>
using namespace std;

class XShell {
	public:
		XShell();
		void begin();
	protected:
		virtual void parseCommand() =0;
		
};
XShell::XShell() {
}
void XShell::begin() {
	while(true) {
		cout << "% ";
		parseCommand();
	}
}
