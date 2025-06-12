#include "create_cid.h"
#include <iomanip>
#include <sstream>

using namespace std;

string getCID() {
    static int counter;
    ostringstream oss;
    oss << "image" << setfill('0') << setw(2) << counter++;
    return oss.str();
}
