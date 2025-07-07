#define NOMINMAX // Prevents min/max macro conflicts on Windows
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <utility>
#include <limits>
#include <Windows.h>  // For ClearScreen()
#include <conio.h>    // For getch() to capture keyboard input
#include <boost/tuple/tuple.hpp>
#include <variant>

#include "vector.hpp" // Custom vector class definitions
#include "gnuplot-iostream.h"

#define UPPER_MENU_BOUND 9 // Upper bound for valid menu option
#define LOWER_MENU_BOUND 0 // Lower bound for valid menu option

// Structure to store user vector input
struct vectorInput
{
    Vector2D vector2D_;   // 2D vector
    Vector3D vector3D_;   // 3D vector
    double x = 0.0, y = 0.0, z = 0.0; // Components
    bool is3D = false;    // Flag: true if input is 3D
};

// Structure to store the result of a menu operation
struct selectionResult
{
    using ResultType = std::variant< std::monostate, double, Vector2D, Vector3D, std::pair<vectorInput,vectorInput> >;
    ResultType resultant;
    bool isPlot = false;
    std::pair<bool, std::string> errFlag = { false, "" }; // Error flag and message
};

// Function prototypes

vectorInput readVector(std::string& line);
void setVector(vectorInput& vector);
bool checkNum(std::string& element);
void displayMenu();
selectionResult performSelection(int userSelection);
std::pair<vectorInput, vectorInput> readTwoVectors();
double readScalar();
bool haveSameDimensions(const vectorInput& v1, const vectorInput& v2);
void processResult(int userSelection);
void plotVectors(vectorInput& firstVector, vectorInput& secondVector);
void ClearScreen();

int main()
{
    unsigned int userSelection;

    do
    {
        ClearScreen(); // Clear console
        displayMenu(); // Show menu options

        std::cout << "Option: ";
        std::cin >> userSelection; // Get user option

        if (!std::cin.good()) // Validate input
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            userSelection = -1;
        }
        processResult(userSelection);
    } while (userSelection != 0); // Repeat until user selects exit
    return 0;
}

// Reads a line input and converts to vectorInput
vectorInput readVector(std::string& line)
{
    vectorInput vector;
    std::string x1;
    std::string x2;
    std::string x3;

    std::stringstream ss(line);

    std::getline(ss, x1, ' '); // Get x
    if (!checkNum(x1))
    {
        std::cout << "Enter Integers Only!" << std::endl;
        exit(EXIT_FAILURE);
    }
    vector.x = std::stod(x1);

    std::getline(ss, x2, ' '); // Get y
    if (!checkNum(x2))
    {
        std::cout << "Enter Real Numbers Only!" << std::endl;
        exit(EXIT_FAILURE);
    }
    vector.y = std::stod(x2);

    if (std::getline(ss, x3, ' ')) // Get z if exists
    {
        if (!checkNum(x3))
        {
            std::cout << "Enter Real Numbers Only!" << std::endl;
            exit(EXIT_FAILURE);
        }
        vector.z = std::stod(x3);
        vector.is3D = true;
    }
    setVector(vector); // Populate vector2D_ or vector3D_
    return vector;
}

// Sets Vector2D or Vector3D values based on is3D flag
void setVector(vectorInput& vector)
{
    if (vector.is3D)
    {
        vector.vector3D_.x = vector.x;
        vector.vector3D_.y = vector.y;
        vector.vector3D_.z = vector.z;
    }
    else
    {
        vector.vector2D_.x = vector.x;
        vector.vector2D_.y = vector.y;
    }
}

// Checks if input string is a valid double
bool checkNum(std::string& element)
{
    try
    {
        std::stod(element);
        return true;
    }
    catch (const std::invalid_argument&)
    {
        return false;
    }
}

// Displays the main calculator menu
void displayMenu()
{
    std::cout << "----SIMPLE VECTOR CALCULATOR----" << std::endl;
    std::cout << "1. Vector Addition" << std::endl;
    std::cout << "2. Vector Subtraction" << std::endl;
    std::cout << "3. Vector Multiplication" << std::endl;
    std::cout << "4. Dot Product" << std::endl;
    std::cout << "5. Cross Product (3D only)" << std::endl;
    std::cout << "6. Magnitude" << std::endl;
    std::cout << "7. Angle Between Vectors" << std::endl;
    std::cout << "8. Plot Two Vectors" << std::endl;

    std::cout << "0. Exit" << std::endl;
}

// Processes the selected menu option
selectionResult performSelection(int userSelection)
{
    selectionResult resultant;
    vectorInput firstVector;
    vectorInput secondVector;
    double scalar; // For scalar multiplication or dot product

    switch (userSelection)
    {
    case 1: // Vector Addition
    {
        std::tie(firstVector, secondVector) = readTwoVectors();

        if (!haveSameDimensions(firstVector, secondVector))
        {
            resultant.errFlag.first = true;
            resultant.errFlag.second = "Vectors Have to Be the Same Dimensions";
        }

        if (!resultant.errFlag.first)
        {
            if (firstVector.is3D)
            {
                resultant.resultant = firstVector.vector3D_ + secondVector.vector3D_;
            }
            else
            {
                resultant.resultant = firstVector.vector2D_ + secondVector.vector2D_;
            }
        }

        break;
    }
    case 2: // Vector Subtraction
    {
        std::tie(firstVector, secondVector) = readTwoVectors();

        if (!haveSameDimensions(firstVector, secondVector))
        {
            resultant.errFlag.first = true;
            resultant.errFlag.second = "Vectors Have to Be the Same Dimensions";
        }

        if (!resultant.errFlag.first)
        {
            if (firstVector.is3D)
            {
                resultant.resultant = firstVector.vector3D_ - secondVector.vector3D_;
            }
            else
            {
                resultant.resultant = firstVector.vector2D_ - secondVector.vector2D_;
            }
        }

        break;
    }
    case 3: // Scalar Multiplication
    {       // Squiggly Brackets needed here to declare line in scope of case 3,
        std::string line;
        std::cout << "Enter the vector: ";
        std::getline(std::cin, line);
        firstVector = readVector(line);

        scalar = readScalar();

        if (firstVector.is3D)
        {
            resultant.resultant = firstVector.vector3D_ * scalar;
        }
        else
        {
            resultant.resultant = firstVector.vector2D_ * scalar;
        }

        break;
    }
    case 4: // Dot Product
    {
        std::tie(firstVector, secondVector) = readTwoVectors();

        if (!haveSameDimensions(firstVector, secondVector))
        {
            resultant.errFlag.first = true;
            resultant.errFlag.second = "Vectors Have to Be the Same Dimensions";
        }

        if (!resultant.errFlag.first)
        {
            if (firstVector.is3D)
            {
                resultant.resultant = firstVector.vector3D_.dotProduct(secondVector.vector3D_);
            }
            else
            {
                resultant.resultant = firstVector.vector2D_.dotProduct(secondVector.vector2D_);
            }
        }

        break;
    }
    case 5: // Cross Product (3D only)
    {
        std::tie(firstVector, secondVector) = readTwoVectors();

        if (!haveSameDimensions(firstVector, secondVector))
        {
            resultant.errFlag.first = true;
            resultant.errFlag.second = "Vectors Have to Be the Same Dimensions";
        }

        if (!resultant.errFlag.first)
        {
            if (firstVector.is3D)
            {
                resultant.resultant = firstVector.vector3D_.crossProduct(secondVector.vector3D_);
            }
            else
                std::cout << "Cross Product Only On 3D vectors."; //set error flag
        }

        break;
    }
    case 6: // Magnitude
    {
        std::string line;
        std::cout << "Enter the vector: ";
        std::getline(std::cin, line);
        firstVector = readVector(line);

        if (firstVector.is3D)
        {
            resultant.resultant = firstVector.vector3D_.magnitude();
        }
        else
            resultant.resultant = firstVector.vector2D_.magnitude();

        break;
    }
    case 7: // Angle Between two Vectors
    {
        std::tie(firstVector, secondVector) = readTwoVectors();

        if (!haveSameDimensions(firstVector, secondVector))
        {
            resultant.errFlag.first = true;
            resultant.errFlag.second = "Vectors Have to Be the Same Dimensions";
        }
        else
        {
            if (firstVector.is3D)
            {
                resultant.resultant = firstVector.vector3D_.angleBetween(secondVector.vector3D_);
            }
            else
            {
                resultant.resultant = firstVector.vector2D_.angleBetween(secondVector.vector2D_);
            }
        }
        break;
    }
    case 8: // Plot Two Vectors
    {
        std::tie(firstVector, secondVector) = readTwoVectors();

        if (!haveSameDimensions(firstVector, secondVector))
        {
            resultant.errFlag.first = true;
            resultant.errFlag.second = "Vectors Have to Be the Same Dimensions";
        }
        else
        {
                resultant.resultant = std::make_pair(firstVector,secondVector);
        }
        break;
    }
    }

    return resultant;
}

void plotVectors(vectorInput& firstVector, vectorInput& secondVector) {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    //Plane Customizations
    gp << "set title 'Normalized Vector Plot' font 'Helvetica, 15'\n";
    gp << "set xlabel 'X axis' font 'Helvetica, 12'\n";
    gp << "set ylabel 'Y axis' font 'Helvetica, 12'\n";
    gp << "set zlabel 'Z axis' font 'Helvetica, 12'\n";
    gp << "set grid\n";
    gp << "set xtics 0.5\n";
    gp << "set ytics 0.5\n";
    gp << "set ztics 0.5\n";
    gp << "set xrange[-1:1]\n";
    gp << "set yrange[-1:1]\n";
    gp << "set zrange[-1:1]\n";
    gp << "set zeroaxis lt 1 linecolor -1 linewidth 1.000\n";

    if (!firstVector.is3D) {
        Vector2D resultantVec = firstVector.vector2D_ + secondVector.vector2D_;
        firstVector.vector2D_ = firstVector.vector2D_.normalize();
        secondVector.vector2D_ = secondVector.vector2D_.normalize();
        resultantVec = resultantVec.normalize();

        std::vector<boost::tuple<double,double,double,double>> vector1 = { {0,0,firstVector.vector2D_.x , firstVector.vector2D_.y} };
        std::vector<boost::tuple<double,double,double,double>> vector2 = { {0,0,secondVector.vector2D_.x , secondVector.vector2D_.y} };
        std::vector<boost::tuple<double,double,double,double>> vector3 = { {0,0,resultantVec.x , resultantVec.y} };

        gp << "plot '-' with vectors title 'Vector 1' lw 2,"
            << "'-' with vectors title 'Vector 2' lw 2,"
            << "'-' with vectors title 'Resultant Vector' lw 3\n";



        gp.send(vector1);
        gp.send(vector2);
        gp.send(vector3);

    }
    else {
        Vector3D resultantVec = (firstVector.vector3D_ + secondVector.vector3D_).normalize();
        firstVector.vector3D_ = firstVector.vector3D_.normalize();
        secondVector.vector3D_ = secondVector.vector3D_.normalize();

        std::vector<boost::tuple<double, double, double, double,double,double>> vector1 = { {0,0,0, firstVector.vector3D_.x , firstVector.vector3D_.y, firstVector.vector3D_.z} };
        std::vector<boost::tuple<double, double, double, double,double,double>> vector2 = { {0,0,0, secondVector.vector3D_.x , secondVector.vector3D_.y, secondVector.vector3D_.z} };
        std::vector<boost::tuple<double, double, double, double,double,double>> vector3 = { {0,0,0, resultantVec.x , resultantVec.y, resultantVec.z} };

        gp << "splot '-' with vectors title 'Vector 1' lw 2,"
            << "'-' with vectors title 'Vector 2' lw 2,"
            << "'-' with vectors title 'Resultant Vector' lw 3\n";

        gp.send(vector1);
        gp.send(vector2);
        gp.send(vector3);
    }

#ifdef _WIN32
    // For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
    // the gnuplot window doesn't get closed.
    std::cout << "Press enter to exit." << std::endl;
    std::cin.get();
#endif

}

std::pair<vectorInput, vectorInput> readTwoVectors()
{
    std::string line;
    vectorInput v1;
    vectorInput v2;

    std::cout << "Enter the First vector: ";
    std::getline(std::cin, line);
    v1 = readVector(line);

    std::cout << "Enter the Second vector: ";
    std::getline(std::cin, line);
    v2 = readVector(line);

    return { v1, v2 };
}

double readScalar()
{
    double scalar;
    while (true)
    {
        std::cout << "Enter The Scalar: ";
        std::cin >> scalar;

        if (std::cin.good())
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return scalar;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid integer.\n";
        }
    }
}

bool haveSameDimensions(const vectorInput& v1, const vectorInput& v2)
{
    return (v1.is3D == v2.is3D);
}

void processResult(int userSelection)
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if ((userSelection > LOWER_MENU_BOUND) && (userSelection < UPPER_MENU_BOUND)) // Check valid bounds
    {
        auto result = performSelection(userSelection); // Process option

        if (result.errFlag.first) // Handle errors
        {
            ClearScreen();
            std::cout << result.errFlag.second << std::endl;
        }
        else if (std::holds_alternative<std::pair< vectorInput, vectorInput>>(result.resultant))
        {
            vectorInput v1 = std::get<std::pair< vectorInput, vectorInput>>(result.resultant).first;
            vectorInput v2 = std::get<std::pair< vectorInput, vectorInput>>(result.resultant).second;

            plotVectors(v1,v2);
        }
        else
        {
            if (std::holds_alternative<double>(result.resultant)) // Show scalar result
            {
                ClearScreen();
                std::cout << "Resultant Scalar: " << "[" << get<double>(result.resultant) << "]" << std::endl;
                _getch();
            }
            else
            {
                if (std::holds_alternative<Vector3D>(result.resultant)) // Show 3D vector result
                {
                    ClearScreen();
                    std::cout << "Resultant Vector: " << "[" << get<Vector3D>(result.resultant).x << ", " << get<Vector3D>(result.resultant).y << ", " << get<Vector3D>(result.resultant).z << "]" << std::endl;
                    _getch();
                }
                else // Show 2D vector result
                {
                    ClearScreen();
                    std::cout << "Resultant Vector: " << "[" << get<Vector2D>(result.resultant).x << ", " << get<Vector2D>(result.resultant).y << "]" << std::endl;
                    _getch();
                }
            }
        }

        std::cin.clear();
        std::cin.ignore(10000, '\n'); // Clear any extra input
    }
}

// Clears the console screen
void ClearScreen()
{
    HANDLE hStdOut;                  // Handle to standard output
    CONSOLE_SCREEN_BUFFER_INFO csbi; // Info about console buffer
    DWORD count;                     // Characters written
    DWORD cellCount;                 // Total cells in console
    COORD homeCoords = { 0, 0 };       // Top-left corner

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return;

    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
        return;

    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count))
        return;

    if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count))
        return;

    SetConsoleCursorPosition(hStdOut, homeCoords);
}
