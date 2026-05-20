#pragma once

#include <optional>
#include <string>

namespace app
{
    //void run(int argc, char** argv);

    struct Request{
        std::string operation;
        int value1;
        std::optional<int> value2;
    };

    class JsonParser{
        public:
        Request parse(int argc, char** argv) const;
    };

    class Calculator{
        public:
        int calculate(const Request& request) const;
    };

    class Printer{
        public:
        void print(const Request& request, int result) const;
    };

    class CalculatorApp{
        public:
        int run(int argc, char** argv);

        private:
        JsonParser parser_;
        Calculator calculator_;
        Printer printer_;
    };

    
}
