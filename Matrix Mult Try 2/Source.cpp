#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<chrono>
#include<vector>
using namespace std;

//struct Timer
//{
//    std::chrono::time_point<std::chrono::steady_clock> start, end;
//    std::chrono::duration<float> duration;
//
//    Timer()
//    {
//        start = std::chrono::high_resolution_clock::now();
//    }
//
//    ~Timer()
//    {
//        end = std::chrono::high_resolution_clock::now();
//        duration = end - start;
//
//        float ms = duration.count() * 1000.0f;
//        //std::cout << "Timer took " << ms << "ms" << std::endl;
//
//        //output info to a file
//        fstream timeTXTFile;
//        timeTXTFile.open("Time.txt", ios::app);
//        if (timeTXTFile.is_open())
//        {
//            timeTXTFile << ms << std::endl;
//            timeTXTFile.close();
//        }
//    }
//};

class Timer
{
public:
    Timer(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        long long duration = end - start;
        //std::cout << m_Name << ": " << duration << "ms\n";


        //output info to a file
        fstream timeTXTFile;
        timeTXTFile.open("Time.txt", ios::app);
        if (timeTXTFile.is_open())
        {
            if (m_Name == "StrassenMatrixMult")
            {
                timeTXTFile << m_Name << ": " << duration << std::endl;
                timeTXTFile << "======================================" << std::endl;
                timeTXTFile.close();
            }
            else
            {
                timeTXTFile << m_Name << ": " << duration << std::endl;
                timeTXTFile.close();
            }
        }

        m_Stopped = true;
    }
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};

//constant variables
const int ARRAYSIZE = 256;
const int NUMBEROFITERATIONS = 25;
const int MIN_VALUE = -9;
const int MAX_VALUE = 9;

//function prototypes

//array versions of Matrix functions********************* Delete me ****************************************
//int(*(createRandomizeMatrixA)())[ARRAYSIZE];
//int(*(createRandomizeMatrixB)())[ARRAYSIZE];
//void classicMatrixMult(int matrixA[ARRAYSIZE][ARRAYSIZE], int matrixB[ARRAYSIZE][ARRAYSIZE], int resultMatrix[ARRAYSIZE][ARRAYSIZE]);
//void naiveDivideAndConquer(int matrixA[ARRAYSIZE][ARRAYSIZE], int matrixB[ARRAYSIZE][ARRAYSIZE], int resultMatrix[ARRAYSIZE][ARRAYSIZE]);
//void naiveHelper(int matrixA[ARRAYSIZE][ARRAYSIZE], int matrixB[ARRAYSIZE][ARRAYSIZE], int resultMatrix[ARRAYSIZE][ARRAYSIZE], int row);
//void StrassenMatrixMult(int matrixA[ARRAYSIZE][ARRAYSIZE], int matrixB[ARRAYSIZE][ARRAYSIZE], int resultMatrix[ARRAYSIZE][ARRAYSIZE]);
//void displayMatrix(int matrix[ARRAYSIZE][ARRAYSIZE]);


vector<vector<int>> classicMatrixMult(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size);
//vector version of Matrix functions
int generateRandomNumb();
void displayMatrix(vector<vector<int>>& matrix);
bool isPowerOfTwo(int number);
//void convertIntoPowerOfTwo(int matrixA[ARRAYSIZE][ARRAYSIZE]);
vector<vector<int>> generateMatrix(int n);
vector<vector<int>> naiveDivideAndConquer(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int baseCase);
vector<vector<int>> naiveHelper(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size, int baseCase);
void matrixAddition(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size);
void matrixSubtraction(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size);
vector<vector<int>> classicMatrixMult(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix);
vector<vector<int>> StrassenMatrixMult(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int baseCase);
vector<vector<int>> StrassenHelper(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size, int baseCase);


int main()
{
    //creates a seed for random numbers
    unsigned seed = time(0);
    srand(seed);

    ////Static array implementation of Matrices NOT USED
    ////***************************************************************************************
    ////The two matricies being multipied and the resulting matrix
    //int (*matrixA) [ARRAYSIZE];
    //int (*matrixB) [ARRAYSIZE];

    ////The result matricies derived by the diffrent multiplication method
    //int resultMatrixCMM [ARRAYSIZE][ARRAYSIZE];
    //int resultMatrixNDAC [ARRAYSIZE][ARRAYSIZE];
    //int resultMatrixSMM [ARRAYSIZE][ARRAYSIZE];

     ////assigns random values to matrices
    //matrixA = createRandomizeMatrixA();
    //matrixB = createRandomizeMatrixB();

   ////prints maticies for test perposes
   // displayMatrix(matrixA);
   // cout << "-----------------------" << endl;
   // displayMatrix(matrixB);
   // classicMatrixMult(matrixA, matrixB, resultMatrixCMM);
   // cout << "-----------------------" << endl;
   // displayMatrix(resultMatrixCMM);
   // naiveDivideAndConquer(matrixA, matrixB, resultMatrixNDAC);
   // cout << "-----------------------" << endl;
   // displayMatrix(resultMatrixNDAC);
   ////***************************************************************************************
    
   
   //takes arraysize and converts it to the next closest power of two
    int PWRTWOARRSIZE = pow(2, ceil(log2(ARRAYSIZE)));
    int baseCase = PWRTWOARRSIZE / 4;

    //Dynamic array implementatin of Matricies
    //****************************************************************************************
     //The two matricies being multipied and the resulting matrix
    vector<vector<int>> vMatrixA;
    vector<vector<int>> vMatrixB;

    //The result matricies derived by the diffrent multiplication method
    vector<vector<int>> vResultMatrixCMM;
    vector<vector<int>> vResultMatrixNDAC;
    vector<vector<int>> vResultMatrixSMM;

    //create a 2-d vector inizalized with all 0's
    vMatrixA.assign(PWRTWOARRSIZE, vector<int>(PWRTWOARRSIZE, 0));
    vMatrixB.assign(PWRTWOARRSIZE, vector<int>(PWRTWOARRSIZE, 0));
    vResultMatrixCMM.assign(PWRTWOARRSIZE, vector<int>(PWRTWOARRSIZE, 0));
    vResultMatrixNDAC.assign(PWRTWOARRSIZE, vector<int>(PWRTWOARRSIZE, 0));
    vResultMatrixSMM.assign(PWRTWOARRSIZE, vector<int>(PWRTWOARRSIZE, 0));


    


    //Displays matrices for debug/test purposes
    /*cout << "-----------Matrix A-----------" << endl;
    displayMatrix(vMatrixA);
    cout << endl << "-----------Matrix B-----------" << endl;
    displayMatrix(vMatrixB);
    cout << endl << "-----------Matrix Classic-----------" << endl;
    vResultMatrixCMM = classicMatrixMult(vMatrixA, vMatrixB);
    displayMatrix(vResultMatrixCMM);
    cout << endl << "-----------Matrix Naive-----------" << endl;
    vResultMatrixNDAC = naiveDivideAndConquer(vMatrixA, vMatrixB, vResultMatrixNDAC);
    displayMatrix(vResultMatrixNDAC);
    cout << endl << "-----------Matrix Strassen-----------" << endl;
    vResultMatrixSMM = StrassenMatrixMult(vMatrixA, vMatrixB, vResultMatrixSMM);
    displayMatrix(vResultMatrixSMM);*/
    //****************************************************************************************

   


    cout << "Program is running...";

    for (int i = 0; i < NUMBEROFITERATIONS; i++)
    {
        //initialize values for matrices with random values
        vMatrixA = generateMatrix(PWRTWOARRSIZE);
        vMatrixB = generateMatrix(PWRTWOARRSIZE);

        //Diffrent implementations of matrix mult
        classicMatrixMult(vMatrixA, vMatrixB, vResultMatrixCMM);
        naiveDivideAndConquer(vMatrixA, vMatrixB, vResultMatrixNDAC, baseCase);
        StrassenMatrixMult(vMatrixA, vMatrixB, vResultMatrixSMM, baseCase);

        //DEBUG: check that outputs are done correctly
        //if (i < 3)
        //{
        //    cout << endl << "=================Iteration " << i << " =================" << endl;
        //    //Displays matrices for debug/test purposes
        //    cout << "-----------Matrix A-----------" << endl;
        //    displayMatrix(vMatrixA);
        //    cout << endl << "-----------Matrix B-----------" << endl;
        //    displayMatrix(vMatrixB);
        //    cout << endl << "-----------Matrix Classic-----------" << endl;
        //    displayMatrix(vResultMatrixCMM);
        //    cout << endl << "-----------Matrix Naive-----------" << endl;
        //    displayMatrix(vResultMatrixNDAC);
        //    cout << endl << "-----------Matrix Strassen-----------" << endl;
        //    displayMatrix(vResultMatrixSMM);
        //}
        // 
        //DEBUG: Tests each base case in order to find which produces best runtimes
        /*while (baseCase < PWRTWOARRSIZE)
        {
            cout << baseCase << endl;
            naiveDivideAndConquer(vMatrixA, vMatrixB, vResultMatrixNDAC, baseCase);
            StrassenMatrixMult(vMatrixA, vMatrixB, vResultMatrixSMM, baseCase);
            baseCase *= 2;
        }
        baseCase = 64;*/
    }

    //take runtimes from Time.txt and average them
    fstream averageTimes;
    fstream runTimes;
    runTimes.open("Time.txt", ios::in);
    averageTimes.open("Averages.txt", ios::app);
    float totalTimeClassic = 0;
    float totalTimeNaive = 0;
    float totalTimeStrassen = 0;
    if (averageTimes.is_open() && runTimes.is_open())
    {
        string line;
        int lineCountClassic = 0;
        int lineCountNaive = 0;
        int lineCountStrassen = 0;
        while (getline(runTimes, line))
        {
            if (line[0] == 'c')
            {
                totalTimeClassic += std::stof(line.erase(0, 18));
                lineCountClassic += 1;
            }
            else if (line[0] == 'n')
            {
                totalTimeNaive += std::stof(line.erase(0, 23));
                lineCountNaive += 1;
            }
            else if (line[0] == 'S')
            {
                totalTimeStrassen += std::stof(line.erase(0, 19));
                lineCountStrassen += 1;
            }
        }
        float averageClassic = totalTimeClassic / lineCountClassic;
        averageTimes << "totalTimeClassic: " << std::to_string(totalTimeClassic) << endl;
        averageTimes << "lineCountClassic: " << std::to_string(lineCountClassic) << endl;
        averageTimes << "averageClassic: " << std::to_string(averageClassic) << endl;

        float averageNaive = totalTimeNaive / lineCountNaive;
        averageTimes << "totalTimeNaive: " << std::to_string(totalTimeNaive) << endl;
        averageTimes << "lineCountNaive: " << std::to_string(lineCountNaive) << endl;
        averageTimes << "averageNaive: " << std::to_string(averageNaive) << endl;

        float averageStrassen = totalTimeStrassen / lineCountStrassen;
        averageTimes << "totalTimeStrassen: " << std::to_string(totalTimeStrassen) << endl;
        averageTimes << "lineCountStrassen: " << std::to_string(lineCountStrassen) << endl;
        averageTimes << "averageStrassen: " << std::to_string(averageStrassen) << endl;

        runTimes.close();
        averageTimes.close();
    }

    cout << "Finished!!!" << endl;
}


//overloaded functin that takes 2D std::vector and displays it
void displayMatrix(vector<vector<int>>& matrix)
{
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        for (int j = 0; j < ARRAYSIZE; j++)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
}


//void convertIntoPowerOfTwo(int matrixA[ARRAYSIZE][ARRAYSIZE])
//{
//    int newArraySize = 2;
//    if (!isPowerOfTwo(ARRAYSIZE))
//    {
//        newArraySize = pow(2, ceil(log2(ARRAYSIZE)));
//    }
//}

bool isPowerOfTwo(int number)
{
    if (ceil(log2(number)) == floor(log2(number)))
    {
        cout << number << " is a power of 2" << endl;
        return true;
    }
    else
    {
        cout << number << " is not a power of 2" << endl;
        return false;
    }
}


vector<vector<int>> generateMatrix(int n)
{
    vector<vector<int>> matrix(n, vector<int>(n, 0));
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        for (int j = 0; j < ARRAYSIZE; j++)
        {
            matrix[i][j] = generateRandomNumb();
        }
    }
    return matrix;
}


int generateRandomNumb()
{
    int randomNum = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
    return randomNum;
}




vector<vector<int>> classicMatrixMult(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix)
{
    Timer timer("classicMatrixMult");

    for (int i = 0; i < ARRAYSIZE; i++)
    {
        for (int j = 0; j < ARRAYSIZE; j++)
        {
            resultMatrix[i][j] = 0;
            for (int k = 0;k < ARRAYSIZE; k++)
            {
                resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return resultMatrix;
}

vector<vector<int>> classicMatrixMult(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix,int size)
{
    //Timer timer("classicMatrixMult");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            resultMatrix[i][j] = 0;
            for (int k = 0;k < size; k++)
            {
                resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return resultMatrix;
}

vector<vector<int>> naiveDivideAndConquer(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int baseCase)
{
    Timer timer("naiveDivideAndConquer");

    int PWRTWOARRSIZE = pow(2, ceil(log2(ARRAYSIZE)));

    return naiveHelper(matrixA, matrixB, resultMatrix, PWRTWOARRSIZE, baseCase);
}

vector<vector<int>> naiveHelper(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size, int baseCase)
{
    if (size == 2)
    {
        resultMatrix[0][0] = (matrixA[0][0] * matrixB[0][0]) + (matrixA[0][1] * matrixB[1][0]);
        resultMatrix[0][1] = (matrixA[0][0] * matrixB[0][1]) + (matrixA[0][1] * matrixB[1][1]);
        resultMatrix[1][0] = (matrixA[1][0] * matrixB[0][0]) + (matrixA[1][1] * matrixB[1][0]);
        resultMatrix[1][1] = (matrixA[1][0] * matrixB[0][1]) + (matrixA[1][1] * matrixB[1][1]);
    }
    else if (size <= baseCase)
    {
        resultMatrix = classicMatrixMult(matrixA, matrixB, resultMatrix,size);
    }
    else
    {
        size /= 2;
        vector<int> innerVector(size, 0);
        vector<vector<int>>
            a11(size, innerVector), a12(size, innerVector), a21(size, innerVector), a22(size, innerVector),
            b11(size, innerVector), b12(size, innerVector), b21(size, innerVector), b22(size, innerVector),
            c11(size, innerVector), c12(size, innerVector), c21(size, innerVector), c22(size, innerVector),
            aResult(size, innerVector), bResult(size, innerVector);


        /*vector<int> innerVector; innerVector.resize(size);
        vector<vector<int>> a11, a12, a21, a22, b11, b12, b21, b22, c11, c12, c21, c22, aResult, bResult;
        a11.resize(size, innerVector);
        a12.resize(size, innerVector);
        a21.resize(size, innerVector);
        a22.resize(size, innerVector);
        b11.resize(size, innerVector);
        b12.resize(size, innerVector);
        b21.resize(size, innerVector);
        b22.resize(size, innerVector);
        c11.resize(size, innerVector);
        c12.resize(size, innerVector);
        c21.resize(size, innerVector);
        c22.resize(size, innerVector);
        aResult.resize(size, innerVector);
        bResult.resize(size, innerVector);*/

        ////11
        //for (int i = 0; i < size / 2; i++)
        //{
        //    for (int j = 0; j < size / 2; j++)
        //    {
        //        a11[i][j] = matrixA[i][j];
        //        b11[i][j] = matrixA[i][j];
        //    }
        //}

        ////12
        //for (int i = (size / 2) + 1; i < size; i++)
        //{
        //    for (int j = 0; j < size / 2; j++)
        //    {
        //        a12[i][j] = matrixA[i][j];
        //        b12[i][j] = matrixA[i][j];
        //    }
        //}

        ////21
        //for (int i = 0; i < size / 2; i++)
        //{
        //    for (int j = (size / 2) + 1; j < size; j++)
        //    {
        //        a21[i][j] = matrixA[i][j];
        //        b21[i][j] = matrixA[i][j];
        //    }
        //}

        ////22
        //for (int i = (size / 2) + 1; i < size; i++)
        //{
        //    for (int j = (size / 2) + 1; j < size; j++)
        //    {
        //        a22[i][j] = matrixA[i][j];
        //        b22[i][j] = matrixA[i][j];
        //    }
        //}


        //dividing the matrices into sub-matrices:
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                a11[i][j] = matrixA[i][j];
                a12[i][j] = matrixA[i][j + size];
                a21[i][j] = matrixA[i + size][j];
                a22[i][j] = matrixA[i + size][j + size];

                b11[i][j] = matrixB[i][j];
                b12[i][j] = matrixB[i][j + size];
                b21[i][j] = matrixB[i + size][j];
                b22[i][j] = matrixB[i + size][j + size];
            }
        }

        //c11 = a11 * b11 + a12 * b21
        naiveHelper(a11, b11, aResult, size, baseCase);
        naiveHelper(a12, b21, bResult, size,baseCase);
        matrixAddition(aResult, bResult, c11, size);

        //c12 = a11 * b12 + a12 * b22
        naiveHelper(a11, b12, aResult, size, baseCase);
        naiveHelper(a12, b22, bResult, size, baseCase);
        matrixAddition(aResult, bResult, c12, size);

        //c21 = a21 * b11 + a22 * b21
        naiveHelper(a21, b11, aResult, size, baseCase);
        naiveHelper(a22, b21, bResult, size,baseCase);
        matrixAddition(aResult, bResult, c21, size);

        //c22 = a21 * b12 + a22 * b22
        naiveHelper(a21, b12, aResult, size, baseCase);
        naiveHelper(a22, b22, bResult, size, baseCase);
        matrixAddition(aResult, bResult, c22, size);

        // Grouping the results obtained in a single matrix:
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                resultMatrix[i][j] = c11[i][j];
                resultMatrix[i][j + size] = c12[i][j];
                resultMatrix[i + size][j] = c21[i][j];
                resultMatrix[i + size][j + size] = c22[i][j];
            }
        }
    }

    return resultMatrix;
}



vector<vector<int>> StrassenMatrixMult(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int baseCase)
{
    Timer timer("StrassenMatrixMult");

    int PWRTWOARRSIZE = pow(2, ceil(log2(ARRAYSIZE)));

    return StrassenHelper(matrixA, matrixB, resultMatrix, PWRTWOARRSIZE, baseCase);
}

vector<vector<int>> StrassenHelper(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size, int baseCase)
{
    if (size == 2)
    {
        resultMatrix[0][0] = (matrixA[0][0] * matrixB[0][0]) + (matrixA[0][1] * matrixB[1][0]);
        resultMatrix[0][1] = (matrixA[0][0] * matrixB[0][1]) + (matrixA[0][1] * matrixB[1][1]);
        resultMatrix[1][0] = (matrixA[1][0] * matrixB[0][0]) + (matrixA[1][1] * matrixB[1][0]);
        resultMatrix[1][1] = (matrixA[1][0] * matrixB[0][1]) + (matrixA[1][1] * matrixB[1][1]);
    }
    else if (size <= baseCase)
    {
        resultMatrix = classicMatrixMult(matrixA, matrixB, resultMatrix, size);
    }
    else
    {
        size /= 2;
        
        vector<int> innerVector(size, 0);
        vector<vector<int>>
            a11(size, innerVector), a12(size, innerVector), a21(size, innerVector), a22(size, innerVector),
            b11(size, innerVector), b12(size, innerVector), b21(size, innerVector), b22(size, innerVector),
            c11(size, innerVector), c12(size, innerVector), c21(size, innerVector), c22(size, innerVector),
            p1(size, innerVector), p2(size, innerVector), p3(size, innerVector), p4(size, innerVector),
            p5(size, innerVector), p6(size, innerVector), p7(size, innerVector),
            aResult(size, innerVector), bResult(size, innerVector);



        /*vector<int> innerVector; innerVector.resize(size);
        vector<vector<int>> a11, a12, a21, a22, b11, b12, b21, b22, c11, c12, c21, c22, p1, p2, p3, p4, p5, p6, p7, aResult, bResult;
        a11.resize(size, innerVector);
        a12.resize(size, innerVector);
        a21.resize(size, innerVector);
        a22.resize(size, innerVector);
        b11.resize(size, innerVector);
        b12.resize(size, innerVector);
        b21.resize(size, innerVector);
        b22.resize(size, innerVector);
        c11.resize(size, innerVector);
        c12.resize(size, innerVector);
        c21.resize(size, innerVector);
        c22.resize(size, innerVector);
        p1.resize(size, innerVector);
        p2.resize(size, innerVector);
        p3.resize(size, innerVector);
        p4.resize(size, innerVector);
        p5.resize(size, innerVector);
        p6.resize(size, innerVector);
        p7.resize(size, innerVector);
        aResult.resize(size, innerVector);
        bResult.resize(size, innerVector);*/


        //sub-matrices are being created
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                a11[i][j] = matrixA[i][j];
                a12[i][j] = matrixA[i][j + size];
                a21[i][j] = matrixA[i + size][j];
                a22[i][j] = matrixA[i + size][j + size];

                b11[i][j] = matrixB[i][j];
                b12[i][j] = matrixB[i][j + size];
                b21[i][j] = matrixB[i + size][j];
                b22[i][j] = matrixB[i + size][j + size];
            }
        }

        //p1 = (a11 + a22) * (b11 + b22)
        matrixAddition(a11, a22, aResult, size);
        matrixAddition(b11, b22, bResult, size);
        StrassenHelper(aResult, bResult, p1, size, baseCase);

        //p2 = (a21 + a22) * (b11)
        matrixAddition(a21, a22, aResult, size);
        StrassenHelper(aResult, b11, p2, size, baseCase);

        //p3 = (a11) * (b12 - b22)
        matrixSubtraction(b12, b22, bResult, size);
        StrassenHelper(a11, bResult, p3, size, baseCase);

        //p4 = (a22) * (b21 - b11)
        matrixSubtraction(b21, b11, bResult, size);
        StrassenHelper(a22, bResult, p4, size, baseCase);

        //p5 = (a11 + a12) * (b22)
        matrixAddition(a11, a12, aResult, size);
        StrassenHelper(aResult, b22, p5, size, baseCase);

        //p6 = (a21 - a11) * (b11 + b12)
        matrixSubtraction(a21, a11, aResult, size);
        matrixAddition(b11, b12, bResult, size);
        StrassenHelper(aResult, bResult, p6, size, baseCase);

        //p7 = (a12 - a22) * (b21 + b22)
        matrixSubtraction(a12, a22, aResult, size);
        matrixAddition(b21, b22, bResult, size);
        StrassenHelper(aResult, bResult, p7, size, baseCase);

        //c11 = p1 + p4 - p5 + p7 => c11 = (p1 + p4) + (p7 - p5)
        matrixAddition(p1, p4, aResult, size);
        matrixSubtraction(p7, p5, bResult, size);
        matrixAddition(aResult, bResult, c11, size);

        //c12 = (p3) + (p5)
        matrixAddition(p3, p5, c12, size);

        //c21 = (p2) + (p4)
        matrixAddition(p2, p4, c21, size);

        //c22 = (p1 - p2) + (p3 + p6)
        matrixSubtraction(p1, p2, aResult, size);
        matrixAddition(p3, p6, bResult, size);
        matrixAddition(aResult, bResult, c22, size);


        // Grouping the results obtained in a single matrix:
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                resultMatrix[i][j] = c11[i][j];
                resultMatrix[i][j + size] = c12[i][j];
                resultMatrix[i + size][j] = c21[i][j];
                resultMatrix[i + size][j + size] = c22[i][j];
            }
        }
    }
    return resultMatrix;
}

void matrixAddition(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            resultMatrix[i][j] = matrixA[i][j] + matrixB[i][j];
        }
    }
}

void matrixSubtraction(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            resultMatrix[i][j] = matrixA[i][j] - matrixB[i][j];
        }
    }
}



////Array version of functions ***********************************************Not Used Delete Me**********************************
//void classicMatrixMult(int matrixA[ARRAYSIZE][ARRAYSIZE], int matrixB[ARRAYSIZE][ARRAYSIZE], int resultMatrix[ARRAYSIZE][ARRAYSIZE])
//{
//    Timer timer("classicMatrixMult");
//
//    for (int i = 0; i < ARRAYSIZE; i++)
//    {
//        for (int j = 0; j < ARRAYSIZE; j++)
//        {
//            resultMatrix[i][j] = 0;
//            for (int k = 0;k < ARRAYSIZE; k++)
//            {
//                resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
//            }
//        }
//    }
//}
//
//void naiveDivideAndConquer(int matrixA[ARRAYSIZE][ARRAYSIZE], int matrixB[ARRAYSIZE][ARRAYSIZE], int resultMatrix[ARRAYSIZE][ARRAYSIZE])
//{
//    //Timer timer;
//
//    //naiveHelper(matrixA, matrixB, resultMatrix, ARRAYSIZE);
//}
//
//void naiveHelper(int matrixA[ARRAYSIZE][ARRAYSIZE], int matrixB[ARRAYSIZE][ARRAYSIZE], int resultMatrix[ARRAYSIZE][ARRAYSIZE], int rowSize)
//{
//    if (rowSize <= 2)
//    {
//        resultMatrix[0][0] = (matrixA[0][0] * matrixB[0][0]) + (matrixA[0][1] * matrixB[1][0]);
//        resultMatrix[0][1] = (matrixA[0][0] * matrixB[0][1]) + (matrixA[0][1] * matrixB[1][1]);
//        resultMatrix[1][0] = (matrixA[1][0] * matrixB[0][0]) + (matrixA[1][1] * matrixB[1][0]);
//        resultMatrix[1][1] = (matrixA[1][0] * matrixB[0][1]) + (matrixA[1][1] * matrixB[1][1]);
//    }
//    else
//    {
//        rowSize /= 2;
//    }
//}
//
//
//void StrassenMatrixMult(int matrixA[ARRAYSIZE][ARRAYSIZE], int matrixB[ARRAYSIZE][ARRAYSIZE], int resultMatrix[ARRAYSIZE][ARRAYSIZE])
//{
//
//}
//
//int(*(createRandomizeMatrixA)())[ARRAYSIZE]
//{
//    static int matrix[ARRAYSIZE][ARRAYSIZE];
//    for (int i = 0; i < ARRAYSIZE; i++)
//    {
//        for (int j = 0; j < ARRAYSIZE; j++)
//        {
//            matrix[i][j] = generateRandomNumb();
//        }
//    }
//    return matrix;
//}
//
//int(*(createRandomizeMatrixB)())[ARRAYSIZE]
//{
//    static int matrix[ARRAYSIZE][ARRAYSIZE];
//    for (int i = 0; i < ARRAYSIZE; i++)
//    {
//        for (int j = 0; j < ARRAYSIZE; j++)
//        {
//            matrix[i][j] = generateRandomNumb();
//        }
//    }
//    return matrix;
//}
//
////overloaded functin that takes a 2D array and displays it
//void displayMatrix(int matrix[ARRAYSIZE][ARRAYSIZE])
//{
//    for (int i = 0; i < ARRAYSIZE; i++)
//    {
//        for (int j = 0; j < ARRAYSIZE; j++)
//        {
//            cout << matrix[i][j] << "\t";
//        }
//        cout << "\n";
//    }
//}
//
////Array version of functions ***********************************************Not Used Delete Me**********************************
