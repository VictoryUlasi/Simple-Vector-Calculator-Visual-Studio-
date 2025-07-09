#define NOMINMAX // Prevents min/max macro conflicts on Windows
#include <iostream>   // For input/output streams
#include <vector>     // For using std::vector container
#include <string>     // For using std::string class
#include <tuple>      // For using std::tuple and std::tie
#include <sstream>    // For using std::stringstream for parsing input
#include <utility>    // For using std::pair
#include <limits>     // For numeric limits when validating input
#include <Windows.h>  // For Windows console functions like ClearScreen()
#include <conio.h>    // For getch() to pause for user input
#include <boost/tuple/tuple.hpp> // For boost::tuple used in Gnuplot data
#include <variant>    // For std::variant type to store multiple result types

#include "vector.hpp" // Custom vector classes (Vector2D, Vector3D, etc.)
#include "gnuplot-iostream.h" // For plotting vectors using Gnuplot

const unsigned int menuItems = 9; // Number Of Items in menu[] array

// Structure to hold user input for a vector
struct vectorInput
{
    Vector2D vector2D_;   // 2D vector instance
    Vector3D vector3D_;   // 3D vector instance
    double x = 0.0, y = 0.0, z = 0.0; // Component values
    bool is3D = false;    // Flag: true if this is a 3D vector
};

// Structure to store the result of an operation selection
struct selectionResult
{
    using ResultType = std::variant<std::monostate, double, Vector2D, Vector3D, std::pair<vectorInput, vectorInput>>;
    ResultType resultant;                           // Holds the result in one of many forms
    bool isPlot = false;                            // Flag: true if this result involves plotting
    std::pair<bool, std::string> errFlag = { false, "" }; // Error flag and message
};

enum class Operation {
    Exit = 0,
    Add,
    Subtract,
    Multiply,
    Dot,
    Cross,
    Magnitude,
    Angle,
    Plot
};

const char* menu[] = {
    "Exit",
    "Vector Addition",
    "Vector Subtraction",
    "Vector Multiplication",
    "Dot Product",
    "Cross Product (3D only)",
    "Magnitude",
    "Angle Between Vectors",
    "Plot Two Vectors"

};

// Function declarations for vector operations
vectorInput readVector(std::string& line);
void setVector(vectorInput& vector);
bool checkNum(std::string& element);
void displayMenu(int numOfItems);
selectionResult performSelection(int userSelection);
std::pair<vectorInput, vectorInput> readTwoVectors();
double readScalar();
bool haveSameDimensions(const vectorInput& v1, const vectorInput& v2);
void processResult(int userSelection);
void plotVectors(vectorInput& firstVector, vectorInput& secondVector);
void ClearScreen();

// Main program entry point
int main()
{
    unsigned int userSelection;

    do
    {
        ClearScreen(); // Clear the console for a clean look
        displayMenu(menuItems); // Show menu options

        std::cout << "Option: ";
        std::cin >> userSelection; // Get user selection

        if (!std::cin.good()) // Check for input errors
        {
            std::cin.clear(); // Clear error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
            userSelection = -1; // Force invalid option
        }

        processResult(userSelection); // Handle the selection
    } while (userSelection != 0); // Loop until user chooses exit

    return 0; // Exit program
}

// Read and parse a single vector input line
vectorInput readVector(std::string& line)
{
    vectorInput vector; // Stores the parsed vector
    std::string x1, x2, x3;

    std::stringstream ss(line); // Stream to parse the line

    std::getline(ss, x1, ' '); // Get X component
    if (!checkNum(x1))
    {
        std::cout << "Enter Integers Only!" << std::endl;
        exit(EXIT_FAILURE);
    }
    vector.x = std::stod(x1); // Convert to double

    std::getline(ss, x2, ' '); // Get Y component
    if (!checkNum(x2))
    {
        std::cout << "Enter Real Numbers Only!" << std::endl;
        exit(EXIT_FAILURE);
    }
    vector.y = std::stod(x2);

    if (std::getline(ss, x3, ' ')) // Get Z component if present
    {
        if (!checkNum(x3))
        {
            std::cout << "Enter Real Numbers Only!" << std::endl;
            exit(EXIT_FAILURE);
        }
        vector.z = std::stod(x3);
        vector.is3D = true; // Mark as 3D vector
    }

    setVector(vector); // Populate Vector2D_ or Vector3D_
    return vector;
}

// Set internal Vector2D_ or Vector3D_ based on is3D flag
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

// Check if a string can be converted to double
bool checkNum(std::string& element)
{
    try
    {
        std::stod(element); // Try to convert to double
        return true;        // Valid number
    }
    catch (const std::invalid_argument&)
    {
        return false;       // Invalid number
    }
}

// Display the main operation menu
void displayMenu(int numOfItems)
{
    std::cout << "----SIMPLE VECTOR CALCULATOR----" << std::endl;
    for (int i = 1; i < numOfItems; ++i) {
        std::cout << i  << ". " << menu[i] << std::endl;
        if (i == numOfItems - 1) {
            std::cout << 0 << ". " << menu[0] << std::endl;
        }
    }
}

// Perform operation based on user selection
selectionResult performSelection(int userSelection)
{
    selectionResult resultant; // Stores the final result
    vectorInput firstVector, secondVector;
    double scalar;

    switch (static_cast<Operation>(userSelection))
    {
    case Operation::Add: // Addition
    {
        std::tie(firstVector, secondVector) = readTwoVectors(); // Get two vectors

        if (!haveSameDimensions(firstVector, secondVector)) // Check dimensions
        {
            resultant.errFlag.first = true;
            resultant.errFlag.second = "Vectors Have to Be the Same Dimensions";
        }

        if (!resultant.errFlag.first)
        {
            if (firstVector.is3D)
                resultant.resultant = firstVector.vector3D_ + secondVector.vector3D_;
            else
                resultant.resultant = firstVector.vector2D_ + secondVector.vector2D_;
        }
        break;
    }
    case Operation::Subtract: // Subtraction
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
                resultant.resultant = firstVector.vector3D_ - secondVector.vector3D_;
            else
                resultant.resultant = firstVector.vector2D_ - secondVector.vector2D_;
        }
        break;
    }
    case Operation::Multiply: // Scalar Multiplication
    {
        std::string line;
        std::cout << "Enter the vector: ";
        std::getline(std::cin, line); // Get vector
        firstVector = readVector(line);

        scalar = readScalar(); // Get scalar

        if (firstVector.is3D)
            resultant.resultant = firstVector.vector3D_ * scalar;
        else
            resultant.resultant = firstVector.vector2D_ * scalar;

        break;
    }
    case Operation::Dot: // Dot Product
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
                resultant.resultant = firstVector.vector3D_.dotProduct(secondVector.vector3D_);
            else
                resultant.resultant = firstVector.vector2D_.dotProduct(secondVector.vector2D_);
        }
        break;
    }
    case Operation::Cross: // Cross Product
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
                resultant.resultant = firstVector.vector3D_.crossProduct(secondVector.vector3D_);
            else
                resultant.errFlag.first = true;
                resultant.errFlag.second = "Cross Product Only Works For 3D vectors.";
        }
        break;
    }
    case Operation::Magnitude: // Magnitude
    {
        std::string line;
        std::cout << "Enter the vector: ";
        std::getline(std::cin, line);
        firstVector = readVector(line);

        if (firstVector.is3D)
            resultant.resultant = firstVector.vector3D_.magnitude();
        else
            resultant.resultant = firstVector.vector2D_.magnitude();

        break;
    }
    case Operation::Angle: // Angle Between
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
                resultant.resultant = firstVector.vector3D_.angleBetween(secondVector.vector3D_);
            else
                resultant.resultant = firstVector.vector2D_.angleBetween(secondVector.vector2D_);
        }
        break;
    }
    case Operation::Plot: // Plot Two Vectors
    {
        std::tie(firstVector, secondVector) = readTwoVectors();

        if (!haveSameDimensions(firstVector, secondVector))
        {
            resultant.errFlag.first = true;
            resultant.errFlag.second = "Vectors Have to Be the Same Dimensions";
        }
        else
        {
            resultant.resultant = std::make_pair(firstVector, secondVector);
        }
        break;
    }
    }

    return resultant; // Return the result of operation
}

// Plot two vectors using Gnuplot
void plotVectors(vectorInput& firstVector, vectorInput& secondVector) {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\""); // Path to Gnuplot executable

    // Set up plot labels, axes, and ranges
    gp << "set title 'Normalized Vector Plot' font 'Helvetica, 15'\n";
    gp << "set xlabel 'X axis' font 'Helvetica, 12'\n";
    gp << "set ylabel 'Y axis' font 'Helvetica, 12'\n";
    gp << "set zlabel 'Z axis' font 'Helvetica, 12'\n";
    gp << "set grid\n";
    gp << "set size square\n";
    gp << "set view 50, 50\n";
    //gp << "set border 4095\n";
    gp << "set xtics 0.5\n";
    gp << "set ytics 0.5\n";
    gp << "set ztics 0.5\n";
    gp << "set xrange[-1:1]\n";
    gp << "set yrange[-1:1]\n";
    gp << "set zrange[-1:1]\n";
    gp << "set zeroaxis lt 1 linecolor -1 linewidth 1.000\n";

    if (!firstVector.is3D) {
        // Normalize and prepare 2D vectors for plotting
        Vector2D resultantVec = firstVector.vector2D_ + secondVector.vector2D_;
        firstVector.vector2D_ = firstVector.vector2D_.normalize();
        secondVector.vector2D_ = secondVector.vector2D_.normalize();
        resultantVec = resultantVec.normalize();

        std::vector<boost::tuple<double, double, double, double>> vector1 = { {0, 0, firstVector.vector2D_.x, firstVector.vector2D_.y} };
        std::vector<boost::tuple<double, double, double, double>> vector2 = { {0, 0, secondVector.vector2D_.x, secondVector.vector2D_.y} };
        std::vector<boost::tuple<double, double, double, double>> vector3 = { {0, 0, resultantVec.x, resultantVec.y} };

        gp << "plot '-' with vectors title 'Vector 1' lw 2,"
            << "'-' with vectors title 'Vector 2' lw 2,"
            << "'-' with vectors title 'Resultant Vector' lw 3\n";

        gp.send(vector1);
        gp.send(vector2);
        gp.send(vector3);
    }
    else {
        // Normalize and prepare 3D vectors for plotting
        Vector3D resultantVec = (firstVector.vector3D_ + secondVector.vector3D_).normalize();
        firstVector.vector3D_ = firstVector.vector3D_.normalize();
        secondVector.vector3D_ = secondVector.vector3D_.normalize();

        std::vector<boost::tuple<double, double, double, double, double, double>> vector1 = { {0, 0, 0, firstVector.vector3D_.x, firstVector.vector3D_.y, firstVector.vector3D_.z} };
        std::vector<boost::tuple<double, double, double, double, double, double>> vector2 = { {0, 0, 0, secondVector.vector3D_.x, secondVector.vector3D_.y, secondVector.vector3D_.z} };
        std::vector<boost::tuple<double, double, double, double, double, double>> vector3 = { {0, 0, 0, resultantVec.x, resultantVec.y, resultantVec.z} };

        gp << "splot '-' with vectors title 'Vector 1' lw 2,"
            << "'-' with vectors title 'Vector 2' lw 2,"
            << "'-' with vectors title 'Resultant Vector' lw 3\n";

        gp.send(vector1);
        gp.send(vector2);
        gp.send(vector3);
    }

#ifdef _WIN32
    // Keep window open on Windows until user hits Enter
    std::cout << "Press enter to exit." << std::endl;
    std::cin.get();
#endif
}

// Prompt user for two vectors
std::pair<vectorInput, vectorInput> readTwoVectors()
{
    std::string line;
    vectorInput v1, v2;

    std::cout << "Enter the First vector: ";
    std::getline(std::cin, line);
    v1 = readVector(line);

    std::cout << "Enter the Second vector: ";
    std::getline(std::cin, line);
    v2 = readVector(line);

    return { v1, v2 };
}

// Prompt user for a scalar value
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

// Check if two vectors have same dimensions (2D vs 3D)
bool haveSameDimensions(const vectorInput& v1, const vectorInput& v2)
{
    return (v1.is3D == v2.is3D);
}

// Process result after a valid selection
void processResult(int userSelection)
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if ((userSelection > 0) && (userSelection < menuItems))
    {
        auto result = performSelection(userSelection);

        if (result.errFlag.first)
        {
            ClearScreen();
            std::cout << result.errFlag.second << std::endl;
        }
        else if (std::holds_alternative<std::pair<vectorInput, vectorInput>>(result.resultant))
        {
            vectorInput v1 = std::get<std::pair<vectorInput, vectorInput>>(result.resultant).first;
            vectorInput v2 = std::get<std::pair<vectorInput, vectorInput>>(result.resultant).second;

            plotVectors(v1, v2);
        }
        else // IMPLEMENT WITH STD::VISIT LATER.
        {
            if (std::holds_alternative<double>(result.resultant))
            {
                ClearScreen();
                std::cout << "Resultant Scalar: [" << get<double>(result.resultant) << "]" << std::endl;
                _getch();
            }
            else
            {
                if (std::holds_alternative<Vector3D>(result.resultant))
                {
                    ClearScreen();
                    std::cout << "Resultant Vector: [" << get<Vector3D>(result.resultant).x << ", "
                        << get<Vector3D>(result.resultant).y << ", "
                        << get<Vector3D>(result.resultant).z << "]" << std::endl;
                    _getch();
                }
                else
                {
                    ClearScreen();
                    std::cout << "Resultant Vector: [" << get<Vector2D>(result.resultant).x << ", "
                        << get<Vector2D>(result.resultant).y << "]" << std::endl;
                    _getch();
                }
            }
        }

        std::cin.clear();
        std::cin.ignore(10000, '\n'); // Clear extra input
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
