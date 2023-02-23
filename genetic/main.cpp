#include "Genetic.h"


int main()
{
    Genetic gen;
    gen.setup(160, 240, 200, 20, 47, "pic.data", "out.data");
    gen.run();

    return 1;
}
