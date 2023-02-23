#include "Genetic.h"


int Genetic::readRGB()
{
    FILE* f;
    fopen_s(&f, name_read, "rb");
    if (f == NULL)
    {
        puts("ERROR: Can't open file\n");
        return NULL;
    }

    int readBytes = 1;
    pixel temp = {};
    while (true)
    {
        readBytes = fread(&temp.R, sizeof(uint8_t), 1, f);
        readBytes = fread(&temp.G, sizeof(uint8_t), 1, f);
        readBytes = fread(&temp.B, sizeof(uint8_t), 1, f);

        if (readBytes == 0)
        {
            break;
        }

        goal_picture.push_back(temp);
    }

    fclose(f);

    if (goal_picture.size() != SZ)
    {
        puts("ERROR: file type unknown\n");
        return NULL;
    }
}

int Genetic::saveData()
{
    FILE* f;
    fopen_s(&f, name_write, "wb");

    if (f == NULL)
    {
        puts("ERROR: Can't create file\n");
        return NULL;
    }

    for (auto& pix : elements[0].first)
    {
        fwrite(&pix.R, sizeof(uint8_t), 1, f);
        fwrite(&pix.G, sizeof(uint8_t), 1, f);
        fwrite(&pix.B, sizeof(uint8_t), 1, f);
    }

    fclose(f);

    return 1;
}

void Genetic::mutate()
{
    pixel pix = { rand() % 256, rand() % 256, rand() % 256 };
    int a = rand() % (int)(W - 1);
    int b = rand() % (int)(H - 1);
    int x = rand() % (W - a);
    int y = rand() % (H - b);

    for (int i = y; i < y + b; i++)
    {
        for (int j = x; j < x + a; j++)
        {
            child[i * W + j] = pix;
        }
    }
}

void Genetic::rank()
{
    double val = 0, max_val = W * H * sqrt(255 * 255 * 3);

    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        val = 0;
        for (int j = 0; j < SZ; j++)
        {

            val += sqrt(pow((elements[i].first[j].R - goal_picture[j].R), 2) + pow((elements[i].first[j].G - goal_picture[j].G), 2) + pow((elements[i].first[j].B - goal_picture[j].B), 2));
        }

        elements[i].second = (max_val - val) / max_val;
    }
    std::sort(std::begin(elements), std::end(elements),
        [&](const auto& a, const auto& b)
        {
            return a.second > b.second;
        });
}

void Genetic::make_love()
{
    int idx = NUMBER_OF_BEST_ELEMENTS;
    for (int i = 0; i < NUMBER_OF_BEST_ELEMENTS; i++)
    {
        for (int j = i + 1; j < NUMBER_OF_BEST_ELEMENTS; j++)
        {
            for (int k = 0; k < SZ; k++)
            {
                if ((rand() % 10) < 5)
                {
                    child[k] = elements[i].first[k];
                }
                else
                {
                    child[k] = elements[j].first[k];
                }
            }

            if ((rand() % 100) < mutation_chance)
            {
                mutate();
            }

            if (idx == POPULATION_SIZE) return;
            elements[idx].first = child;
            idx++;
        }
    }
}

Genetic::Genetic()
{
    name_read = nullptr;
    name_write = nullptr;
}

Genetic::~Genetic()
{
}

void Genetic::setup(int w, int h, int pop_size, int nof_best, int mut_per, const char* n_in, const char* n_out)
{
    W = w;
    H = h;
    SZ = w * h;
    POPULATION_SIZE = pop_size;
    NUMBER_OF_BEST_ELEMENTS = nof_best;
    name_read = n_in;
    name_write = n_out;

    pixel pix = { 0, 0, 0 };
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        std::vector<pixel> tv;
        for (int j = 0; j < SZ; j++)
        {
            tv.push_back(pix);
        }

        std::pair<std::vector<pixel>, double> tmp = { tv, 0 };
        elements.push_back(tmp);
    }

    for (int i = 0; i < SZ; i++)
    {
        child.push_back(pix);
    }

    setup_done = true;
}

void Genetic::run()
{
    if (setup_done == false)
    {
        puts("You need to set up variables first - use setup function!");
        return;
    }

    std::srand(time(NULL));

    if (readRGB() == NULL)
    {
        exit(-1);
    }

    
    int generation = 0;
    while (true)
    {
        rank();

        if (generation % 200 == 0)
        {
            printf("[ ! ]   generation: %d, similarity: %f. Best picture dumped.\n", generation, elements[0].second);
            saveData();
        }

        make_love();
        generation++;
    }
}
