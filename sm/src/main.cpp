#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <stack>
#include <unordered_map>

struct Stack {
public:
    Stack(const size_t stack_size) {
        while (!stack.empty()) {
            stack.pop();
        }

        this->stack_size = stack_size;
    }

    size_t get_stack_size() {
        return this->stack_size;
    }

    void push(const size_t input) {
        this->stack.push(input);
    }

    bool pop() {
        if (this->stack.empty()) {
            return false;
        }

        this->stack.pop();
        return true;
    }

    size_t get_top() {
        if (this->stack.empty()) {
            return 0;
        }

        return this->stack.top();
    }
private:
    size_t stack_size;
    std::stack<size_t> stack;
};

struct Memory {
public:
    Memory(const size_t mem_size) {
        this->memory.clear();
        this->mem_size = mem_size;
    }

    void learn(const size_t value, const size_t mem_address) {
        this->memory[mem_address] = value;
    }

    size_t remember(const size_t mem_address) {
        return this->memory[mem_address];
    }
private:
    std::unordered_map<size_t, size_t> memory;
    size_t mem_size;
};

bool is_lesser(const std::string a, const std::string b) {
    if (a.size() < b.size()) {
        return true;
    }

    if (a.size() > b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] < b[i]) {
            return true;
        }

        if (a[i] > b[i]) {
            return false;
        }
    }

    return false;
}

bool is_unsigned_int(const std::string s) {
    if (s.size() == 0) {
        return false;
    }

    for (const char &c : s) {
        if ('0' <= c && c <= '9') {
            continue;
        }

        return false;
    }

    return true;
}

struct SMInterpreter {
public:
    SMInterpreter() {
        this->stack = nullptr;
        this->memory = nullptr;
        this->input_mode = "";
        this->output_mode = "";
    }

    bool enable_sm(const size_t bit_size) {
        if (this->stack != nullptr) {
            return false;
        }

        this->stack = std::make_shared<Stack>(bit_size);
        
        if (this->memory != nullptr) {
            return false;
        }

        this->memory = std::make_shared<Memory>(bit_size);
        return true;
    }

    bool enable_input(const bool ascii) {
        if (this->input_mode != "") {
            return false;
        }

        if (ascii) {
            this->input_mode = "ascii";
        } else {
            this->input_mode = "int";
        }
        
        return true;
    }

    bool enable_output(const bool ascii) {
        if (this->output_mode != "") {
            return false;
        }

        if (ascii) {
            this->output_mode = "ascii";
        } else {
            this->output_mode = "int";
        }

        return true;
    }

    size_t get_bit_size() {
        return this->stack->get_stack_size();
    }
    
    std::string get_input_mode() {
        return this->input_mode;
    }

    std::string get_output_mode() {
        return this->output_mode;
    }

    bool enabled_sm() {
        return this->stack != nullptr && this->memory != nullptr;
    }

    bool enabled_input() {
        return this->input_mode != "";
    }

    bool enabled_output() {
        return this->output_mode != "";
    }

    void push(const size_t input) {
        this->stack->push(input);
    }

    bool pop() {
        return this->stack->pop();
    }

    size_t get_stack_top() {
        return this->stack->get_top();
    }

    void learn(const size_t value, const size_t mem_address) {
        this->memory->learn(value, mem_address);
    }

    void remember(const size_t mem_address) {
        this->push(this->memory->remember(mem_address));
    }
private:
    std::shared_ptr<Stack> stack;
    std::shared_ptr<Memory> memory;
    std::string input_mode;
    std::string output_mode;
};

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string fn = "";
    for (int i = 1; i < argc; i++) {
        fn += argv[i];
    }

    fn += ".sm";
    std::ifstream src(fn);
    if (!src) {
        std::cout << "File " << fn << " cannot be found!\n";
        return 0;
    }

    std::stringstream code;
    std::string line;
    while (src.is_open() && std::getline(src, line)) {
        code << line << "\n";
    }

    std::string real = code.str();
    if (real.size() > 0) {
        real.erase(std::prev(real.end())); // Remove the ending newline
    }

    std::vector<std::string> instructions = {};
    std::string current_instruction = "";
    for (const char &c : real + "\n") {
        if (c == '\n') {
            instructions.push_back(current_instruction);
            current_instruction = "";

            continue;
        }

        if (c != ' ' && c != '\t') {
            current_instruction += c;
        } else {
            current_instruction += c;
        }
    }

    std::vector<std::vector<std::string>> tokens = {};
    for (const std::string &instruction : instructions) {
        std::vector<std::string> ins_toks = {};
        std::string current_tok = "";
        for (const char &c : instruction + " ") {
            if (c == ' ') {
                if (current_tok != "") {
                    ins_toks.push_back(current_tok);
                }

                current_tok = "";
            } else {
                current_tok += c;
            }
        }

        tokens.push_back(ins_toks);
    }

    size_t pc = 0;
    SMInterpreter sm = SMInterpreter();
    while (pc < tokens.size()) {
        if (tokens[pc].size() == 0) {
            pc++;
            continue;
        }

        std::string ins = tokens[pc][0];
        if (ins[0] == '@') {
            if (ins == "@sm") {
                if (tokens[pc].size() < 2) {
                    std::cout << "error at line " << pc+1 << ": @sm expects 8/16/32/64\n";
                    return 1;
                }

                if (tokens[pc].size() > 2) {
                    std::cout << "error at line " << pc+1 << ": line overload\n";
                    return 2;
                }

                std::string bit_size = tokens[pc][1];
                if (bit_size == "8") {
                    bool success = sm.enable_sm(8);
                    if (!success) {
                        std::cout << "error at line " << pc+1 << ": stack and memory is already loaded\n";
                        return 3;
                    }
                } else if (bit_size == "16") {
                    bool success = sm.enable_sm(16);
                    if (!success) {
                        std::cout << "error at line " << pc+1 << ": stack and memory is already loaded\n";
                        return 4;
                    }
                } else if (bit_size == "32") {
                    bool success = sm.enable_sm(32);
                    if (!success) {
                        std::cout << "error at line " << pc+1 << ": stack and memory is already loaded\n";
                        return 5;
                    }
                } else if (bit_size == "64") {
                    bool success = sm.enable_sm(64);
                    if (!success) {
                        std::cout << "error at line " << pc+1 << ": stack and memory is already loaded\n";
                        return 6;
                    }
                } else {
                    std::cout << "error at line " << pc+1 << ": @sm expects 8/16/32/64\n";
                    return 7;
                }
            } else if (ins == "@output") {
                if (tokens[pc].size() < 2) {
                    std::cout << "error at line " << pc+1 << ": @output expects ascii/int keyword\n";
                    return 28;
                }

                if (tokens[pc].size() > 2) {
                    std::cout << "error at line " << pc+1 << ": line overload\n";
                    return 29;
                }

                if (tokens[pc][1] == "int") {
                    bool success = sm.enable_output(false);
                    if (!success) {
                        std::cout << "error at line " << pc+1 << ": output stream is already loaded\n";
                        return 30;
                    }
                } else if (tokens[pc][1] == "ascii") {
                    bool success = sm.enable_output(true);
                    if (!success) {
                        std::cout << "error at line " << pc+1 << ": output stream is already loaded\n";
                        return 31;
                    }
                } else {
                    std::cout << "error at line " << pc+1 << ": @output expects ascii/int keyword\n";
                    return 32;
                }
            } else {
                std::cout << "error at line " << pc+1 << ": unexpected setting\n";
                return 8;
            }

            pc++;
        } else if (ins == "push") {
            if (!sm.enabled_sm()) {
                std::cout << "error at line " << pc+1 << ": setting '@sm' is not enabled yet\n";
                return 10;
            }

            if (tokens[pc].size() < 2) {
                std::cout << "error at line " << pc+1 << ": expect a " << sm.get_bit_size() << " bit unsigned integer\n";
                return 11;
            }

            if (tokens[pc].size() > 2) {
                std::cout << "error at line " << pc+1 << ": line overload\n";
                return 12;
            }

            std::string pushed = tokens[pc][1];
            if (!is_unsigned_int(pushed)) {
                std::cout << "error at line " << pc+1 << ": expect a " << sm.get_bit_size() << " bit unsigned integer\n";
                return 13;
            }

            if (sm.get_bit_size() == 8) {
                if (!is_lesser(pushed, "256")) {
                    std::cout << "error at line " << pc+1 << ": expect a 8 bit unsigned integer\n";
                    return 14;
                }

                sm.push(std::stoull(pushed));
            } else if (sm.get_bit_size() == 16) {
                if (!is_lesser(pushed, "65536")) {
                    std::cout << "error at line " << pc+1 << ": expect a 16 bit unsigned integer\n";
                    return 15;
                }

                sm.push(std::stoull(pushed));
            } else if (sm.get_bit_size() == 32) {
                if (!is_lesser(pushed, "4294967296")) {
                    std::cout << "error at line " << pc+1 << ": expect a 32 bit unsigned integer\n";
                    return 16;
                }

                sm.push(std::stoull(pushed));
            } else {
                if (!is_lesser(pushed, "18446744073709551616")) {
                    std::cout << "error at line " << pc+1 << ": expect a 64 bit unsigned integer\n";
                    return 17;
                }

                sm.push(std::stoull(pushed));
            }

            pc++;
        } else if (ins == "pop") {
            if (!sm.enabled_sm()) {
                std::cout << "error at line " << pc+1 << ": setting '@sm' is not enabled\n";
                return 18;
            }

            if (tokens[pc].size() > 1) {
                std::cout << "error at line " << pc+1 << ": line overload\n";
                return 19;
            }

            bool success = sm.pop();
            if (!success) {
                std::cout << "error at line " << pc+1 << ": stack is empty\n";
                return 20;
            }

            pc++;
        } else if (ins == "learn") {
            if (!sm.enabled_sm()) {
                std::cout << "error at line " << pc+1 << ": setting '@sm' is not enabled\n";
                return 21;
            }

            if (tokens[pc].size() > 1) {
                std::cout << "error at line " << pc+1 << ": line overload\n";
                return 22;
            }

            size_t value = sm.get_stack_top();
            bool success = sm.pop();
            if (!success) {
                std::cout << "error at line " << pc+1 << ": stack size needed to be at least 2, got 0\n";
                return 23;
            }

            size_t mem_address = sm.get_stack_top();
            success = sm.pop();
            if (!success) {
                std::cout << "error at line " << pc+1 << ": stack size needed to be at least 2, got 1\n";
                return 24;
            }

            sm.learn(value, mem_address);
            pc++;
        } else if (ins == "remember") {
            if (!sm.enabled_sm()) {
                std::cout << "error at line " << pc+1 << ": setting '@sm' is not enabled\n";
                return 25;
            }

            if (tokens[pc].size() > 1) {
                std::cout << "error at line " << pc+1 << ": line overload\n";
                return 26;
            }

            size_t mem_address = sm.get_stack_top();
            bool success = sm.pop();
            if (!success) {
                std::cout << "error at line " << pc+1 << ": stack size needed to be at least 1, got 0\n";
                return 27;
            }

            sm.remember(mem_address);
        } else if (ins == "log") {
            if (!sm.enabled_output()) {
                std::cout << "error at line " << pc+1 << ": setting '@output' is not enabled\n";
                return 33;
            }

            if (!sm.enabled_sm()) {
                std::cout << "error at line " << pc+1 << ": setting '@sm' is not enabled\n";
                return 34;
            }

            if (tokens[pc].size() > 1) {
                std::cout << "error at line " << pc+1 << ": line overload\n";
                return 35;
            }

            size_t printed = sm.get_stack_top();
            bool success = sm.pop();
            if (!success) {
                std::cout << "error at line " << pc+1 << ": stack size needed to be at least 1, got 0\n";
                return 36;
            }

            if (sm.get_output_mode() == "int") {
                std::cout << printed << "\n";
                pc++;
                continue;
            }

            if (printed >= 256) {
                std::cout << "error at line " << pc+1 << ": top of the stack needs to be smaller than 256 in order to print the character in ASCII\n";
                return 37;
            }

            std::cout << (char)printed;
            pc++;
        } else {
            std::cout << "error at line " << pc+1 << ": unexpected instruction\n";
            return 9;
        }
    }

    return 0;
}