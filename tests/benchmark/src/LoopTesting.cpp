#include <cabba/test/test.h>

#include <cabba/ecs/Entity.h>
#include <cabba/ecs/EntityManager.h>
#include <map>
#include <unordered_map>
#include <cstdlib>
#include <vector>

struct TestComponent
{
    int v1;
    int v2;
    int v3;
    int v4;
};

using namespace cabba::test;
using namespace cabba;

class LoopTesting : public cabba::test::Test
{
public:

    TestComponent* components;

    int size = 10000000;

    //std::map<int, int> my_map;
    //std::map<int, int> random_map;
    std::unordered_map<int, int> random_umap;
    std::vector<int> registered;
    std::vector<int> registered_random;

    int *lookup_table;
    int *random_lookup_table;
    int sum = 0;

    void set_up()override
    {
        srand(2);

        components          = new TestComponent[size];
        lookup_table        = new int[size];
        random_lookup_table = new int[size];

        registered.reserve(size);
        registered_random.reserve(size / 2);
        
        for(int i=0; i< size; ++i)
        {
            random_lookup_table[i] = -1;

            if(rand()%2==0)
            {
                //random_map[random_map.size()]   = i;
               // random_umap[random_umap.size()] = i;
                random_lookup_table[i]          = i;
                registered_random.push_back(i);
            }

           // my_map.emplace(i, i);
            lookup_table[i] = i;
            components[i].v1 = i;
            components[i].v2 = i+1;
            components[i].v3 = i+2;
            components[i].v4 = i+3;
            registered.push_back(i);
        }
        printf("%d", registered_random.size());
    }

    void tear_down() override
    {
        delete[] components;
        delete[] lookup_table;
        delete[] random_lookup_table;
        printf("%d ", sum);
    }
};

TEST_F(LoopTesting, testLoop)
{
    for(int i=0; i< size; ++i)
    {
        TestComponent& c = components[i];
        sum += 
            c.v1 + c.v2 +
            c.v3 + c.v4;
    }
}

TEST_F(LoopTesting, test_vector_loop)
{
    for (size_t i = 0; i < registered.size(); ++i)
    {
        int j = registered[i];
        sum +=
            components[j].v1 + components[j].v2 +
            components[j].v3 + components[j].v4;
    }
}

// Ok so the map is slow af
//TEST_F(LoopTesting, TestMapLoop)
//{
//    for (auto& pair : my_map)
//    {
//        int index = pair.second;
//        sum += components[index].v1 + components[index].v2 +
//            components[index].v3 + components[index].v4;
//    }
//}

TEST_F(LoopTesting, lookup_table)
{
    for (int i=0; i< size; ++i)
    {
        int j = lookup_table[i];
        sum += 
            components[j].v1 + components[j].v2 +
            components[j].v3 + components[j].v4;
    }
}

TEST_F(LoopTesting, iterator_loop)
{
    TestComponent* start = components;
    TestComponent* end   = &components[size-1];

    while(start!=end)
    {
        
        sum +=
            start->v1 + start->v2 +
            start->v3 + start->v4;

        start++;
    }
}


// Ok so the map is slow af
//TEST_F(LoopTesting, random_map_loop)
//{
//    for (auto& pair : random_map)
//    {
//        int index = pair.second;
//        sum += components[index].v1 + components[index].v2 +
//            components[index].v3 + components[index].v4;
//    }
//}

//TEST_F(LoopTesting, random_umap)
//{
//    for (auto& pair : random_umap)
//    {
//        int index = pair.second;
//        sum += components[index].v1 + components[index].v2 +
//            components[index].v3 + components[index].v4;
//    }
//}


TEST_F(LoopTesting, random_lookup_table)
{
    for (int i = 0; i < size; ++i)
    {
        int j = lookup_table[i];
        if(j!=-1)
        {
            sum +=
                components[j].v1 + components[j].v2 +
                components[j].v3 + components[j].v4;
        }
    }
}



TEST_F(LoopTesting, random_registered)
{
    for(int j : registered_random)
    {
        sum +=
            components[j].v1 + components[j].v2 +
            components[j].v3 + components[j].v4;
    }
}

TEST_F(LoopTesting, random_packed)
{
    for(int j=0; j< registered_random.size(); ++j)
    {
        sum +=
            components[j].v1 + components[j].v2 +
            components[j].v3 + components[j].v4;
    }
}
