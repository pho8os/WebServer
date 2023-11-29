#include "Cgi.hpp"


int main()
{
    try {
    cgi a(PY);
    a.setEnv();
    a.execute();
    } catch (int a) {
    }
}