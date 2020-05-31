#include "CppUnitTest.h"
#include "main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(tests){
    public :
        TEST_METHOD(test_example)
        {
            std::vector<std::vector<int>> adj_matrix =
            {
                {INF, 3, 3, INF, INF, INF},
                {INF, INF, 2, 3, INF, INF},
                {INF, INF, INF, INF, 2, INF},
                {INF, INF, INF, INF, 4, 2},
                {INF, INF, INF, INF, INF, 3},
                {INF, INF, INF, INF, INF, INF}
            };
            std::vector<std::vector<int>> adj_matrix_stream = get_stream(adj_matrix);
            std::vector<std::vector<int>> correct_answer =
            {
                {INF, 3, 2, INF, INF, INF},
                {INF, INF, 0, 3, INF, INF},
                {INF, INF, INF, INF, 2, INF},
                {INF, INF, INF, INF, 1, 2},
                {INF, INF, INF, INF, INF, 3},
                {INF, INF, INF, INF, INF, INF}
            };
            Assert::IsTrue(adj_matrix_stream == correct_answer);
        }
    };
}