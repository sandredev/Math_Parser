#pragma once

#include <vector>
#include <string>

namespace lexer {
    // Abstract class that represents a DFA
    class DFA { 
        protected:
            int current_state;             

            /* 
               Table of transitions where each column (except the last) is an input and
               each row is an state.

               The last column says if an state is of acceptance or not.
            */
            std::vector<std::vector<int>> transitions_table;

        public:
            DFA();

            // Transitionates from an state to another one according to an input
            virtual void do_transition(char& input) = 0;

            // Says if the current input is accepted by the DFA
            virtual bool is_lexem() const = 0;

            // Method used for testing DFAs
            virtual void test() = 0;

            // In case of working with dynamic casting, this fully cleans the memory of the information
            // stored by any DFA object created
            virtual ~DFA();

            // Restart the DFA to identify a new pattern
            virtual void restart();

            // Says which token the DFA has accepted (in case it accepts more than one)
            virtual std::string which_token_is() = 0;

            // Returns the current state. Used for debugging
            virtual int get_current_state();
    };
}