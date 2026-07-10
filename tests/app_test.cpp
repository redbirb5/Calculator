#include "app.h"
#include "config.h"

#include <gtest/gtest.h>
#include <string>

struct RunResult
{
    int exit_code;
    std::string stdout_text;
    std::string stderr_text;
};

class CalculatorAppTest : public testing::Test
{
  protected:
    RunResult runApp(const char* argument);
};

RunResult CalculatorAppTest::runApp(const char* argument)
{
    app::CalculatorApp calculator_app(app::readConnectionString("config.json"));

    char* argv[] = {
        const_cast<char*>("calculator"),
        const_cast<char*>(argument),
    };
    int argc = 2;

    RunResult res;

    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();

    res.exit_code = calculator_app.run(argc, argv);
    res.stdout_text = testing::internal::GetCapturedStdout();
    res.stderr_text = testing::internal::GetCapturedStderr();

    return res;
}

// Tests for valid arguments

TEST_F(CalculatorAppTest, AddsTwoNumbersFromJson)
{
    RunResult res = runApp("{\"operation\":\"add\",\"value1\":1,\"value2\":2}");

    EXPECT_EQ(res.exit_code, 0);
    EXPECT_EQ(res.stdout_text, "1 + 2 = 3\n");
    EXPECT_EQ(res.stderr_text, "");
}

TEST_F(CalculatorAppTest, SubtractsTwoNumbersFromJson)
{
    RunResult res = runApp("{\"operation\":\"subtract\",\"value1\":188,\"value2\":18}");

    EXPECT_EQ(res.exit_code, 0);
    EXPECT_EQ(res.stdout_text, "188 - 18 = 170\n");
    EXPECT_EQ(res.stderr_text, "");
}

TEST_F(CalculatorAppTest, MultipliesTwoNumbersFromJson)
{
    RunResult res = runApp("{\"operation\":\"multiply\",\"value1\":5,\"value2\":-6}");

    EXPECT_EQ(res.exit_code, 0);
    EXPECT_EQ(res.stdout_text, "5 * (-6) = -30\n");
    EXPECT_EQ(res.stderr_text, "");
}

TEST_F(CalculatorAppTest, DividesTwoNumbersFromJson)
{
    RunResult res = runApp("{\"operation\":\"divide\",\"value1\":15,\"value2\":3}");

    EXPECT_EQ(res.exit_code, 0);
    EXPECT_EQ(res.stdout_text, "15 / 3 = 5\n");
    EXPECT_EQ(res.stderr_text, "");
}

TEST_F(CalculatorAppTest, RaisesNumberToPowerFromJson)
{
    RunResult res = runApp("{\"operation\":\"power\",\"value1\":4,\"value2\":3}");

    EXPECT_EQ(res.exit_code, 0);
    EXPECT_EQ(res.stdout_text, "4 ^ 3 = 64\n");
    EXPECT_EQ(res.stderr_text, "");
}

TEST_F(CalculatorAppTest, CalculatesFactorialFromJson)
{
    RunResult res = runApp("{\"operation\":\"factorial\",\"value1\":5}");

    EXPECT_EQ(res.exit_code, 0);
    EXPECT_EQ(res.stdout_text, "5! = 120\n");
    EXPECT_EQ(res.stderr_text, "");
}

// Tests for calculation errors

TEST_F(CalculatorAppTest, ReturnsErrorOnAdditionOverflow)
{
    RunResult res = runApp("{\"operation\":\"add\",\"value1\":2147483647,\"value2\":1}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_EQ(res.stderr_text, "Error! Type overflow\n");
}

TEST_F(CalculatorAppTest, ReturnsErrorOnDivisionByZero)
{
    RunResult res = runApp("{\"operation\":\"divide\",\"value1\":10,\"value2\":0}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_EQ(res.stderr_text, "Error! Division by zero\n");
}

TEST_F(CalculatorAppTest, ReturnsErrorOnNegativePower)
{
    RunResult res = runApp("{\"operation\":\"power\",\"value1\":2,\"value2\":-3}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_EQ(res.stderr_text, "Error! Raising to a negative power\n");
}

TEST_F(CalculatorAppTest, ReturnsErrorOnNegativeFactorial)
{
    RunResult res = runApp("{\"operation\":\"factorial\",\"value1\":-5}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_EQ(res.stderr_text, "Error! Factorial of a negative number\n");
}

// Tests for input validation errors

TEST_F(CalculatorAppTest, ReturnsErrorOnUnknownOperation)
{
    RunResult res = runApp("{\"operation\":\"unknown\",\"value1\":1,\"value2\":2}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_EQ(res.stderr_text, "Error! Unknown operation\n");
}

TEST_F(CalculatorAppTest, ReturnsErrorWhenValue2IsMissingForBinaryOperation)
{
    RunResult res = runApp("{\"operation\":\"add\",\"value1\":1}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_EQ(res.stderr_text, "Error! Missing required JSON fields: value2\n");
}

TEST_F(CalculatorAppTest, ReturnsErrorWhenValue1IsMissing)
{
    RunResult res = runApp("{\"operation\":\"add\",\"value2\":5}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_EQ(res.stderr_text,
              "Error! Missing required JSON fields: operation or value1\n");
}

TEST_F(CalculatorAppTest, ReturnsErrorWhenOperationIsMissing)
{
    RunResult res = runApp("{\"value1\":1,\"value2\":5}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_EQ(res.stderr_text,
              "Error! Missing required JSON fields: operation or value1\n");
}

TEST_F(CalculatorAppTest, ReturnsErrorWhenWrongTypeForValue1)
{
    RunResult res =
        runApp("{\"operation\":\"add\",\"value1\":\"abc\",\"value2\":5}");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_NE(res.stderr_text.find("Error!"), std::string::npos);
    EXPECT_NE(res.stderr_text.find("type must be number"), std::string::npos);
}

TEST_F(CalculatorAppTest, ReturnsErrorOnInvalidJsonSyntax)
{
    RunResult res = runApp("{bad json");

    EXPECT_EQ(res.exit_code, 1);
    EXPECT_EQ(res.stdout_text, "");
    EXPECT_NE(res.stderr_text.find("Error!"), std::string::npos);
    EXPECT_NE(res.stderr_text.find("parse error"), std::string::npos);
}

TEST_F(CalculatorAppTest, PrintsHelp)
{
    RunResult res = runApp("--help");

    EXPECT_EQ(res.exit_code, 0);
    EXPECT_NE(res.stdout_text.find("Usage:"), std::string::npos);
    EXPECT_NE(res.stdout_text.find("calculator '<json>'"), std::string::npos);
    EXPECT_EQ(res.stderr_text, "");
}
