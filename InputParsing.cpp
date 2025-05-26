#include "InputParsing.h"
#include <bits/stdc++.h>
using namespace std;

InputParsing::InputParsing(map<int, map<string, string>> actionTable,
                           map<int, map<string, int>> gotoTable,
                           map<string, vector<vector<string>>> augment_prod)
    : actionTable(actionTable), gotoTable(gotoTable), augment_prod(augment_prod) {}

void InputParsing::prepareSimulation()
{
  simulationStates.clear();
  currentSimulationStep = 0;

  // Tokenize input
  ifstream inputFile("input.txt");
  if (!inputFile.is_open())
  {
    throw runtime_error("Could not open input.txt file");
  }
  string input;
  getline(inputFile, input);
  inputFile.close();

  vector<string> tokens;
  istringstream iss(input);
  string token;
  cout << "Input Tokens: " << endl;
  while (iss >> token)
  {
    cout << token << " ";
    tokens.push_back(token);
  }
  cout << endl;
  // Initialize first state of stack...
  SimulationState initialState;
  initialState.stateStack.push(0);
  initialState.inputPointer = 0;
  initialState.accepted = false;
  initialState.error = false;
  simulationStates.push_back(initialState);
}

bool InputParsing::hasNextStep() const
{
  if (simulationStates.empty())
    return false;
  return currentSimulationStep < simulationStates.size() - 1 ||
         (!simulationStates.back().accepted && !simulationStates.back().error);
}

void InputParsing::nextStep()
{
  if (!hasNextStep())
    return;

  if (currentSimulationStep == simulationStates.size() - 1)
  {
    // Need to compute next step
    const SimulationState &currentState = simulationStates.back();
    SimulationState newState = currentState;

    int currentStackState = newState.stateStack.top();
    string currentSymbol;

    // Get next input symbol
    ifstream inputFile("input.txt");
    string input;
    if (inputFile.is_open())
    {
      getline(inputFile, input);
      inputFile.close();
    }
    istringstream iss(input);
    string token;
    size_t pos = 0;
    while (iss >> token)
    {
      if (pos == newState.inputPointer)
      {
        currentSymbol = token;
        break;
      }
      pos++;
    }
    if (currentSymbol.empty())
    {
      currentSymbol = "$";
    }

    // Check for valid action
    if (actionTable[currentStackState].count(currentSymbol) == 0)
    {
      newState.error = true;
      simulationStates.push_back(newState);
      currentSimulationStep++;
      return;
    }

    string action = actionTable[currentStackState][currentSymbol];
    newState.currentAction = action;

    if (action == "acc")
    {
      newState.accepted = true;
    }
    else if (action[0] == 's')
    {
      // Shift action
      int nextState = stoi(action.substr(1));
      newState.stateStack.push(nextState);
      newState.symbolStack.push(currentSymbol);
      newState.inputPointer++;
    }
    else if (action[0] == 'r')
    {
      // Reduce action
      int ruleNumber = stoi(action.substr(1));

      // Find the production to reduce by
      bool found = false;
      int currentRule = 0;
      for (const auto &production : augment_prod)
      {
        const string &lhs = production.first;
        for (const auto &rhs : production.second)
        {
          if (currentRule == ruleNumber)
          {
            // Pop symbols from stacks
            for (size_t i = 0; i < rhs.size(); ++i)
            {
              if (!newState.symbolStack.empty())
                newState.symbolStack.pop();
              if (!newState.stateStack.empty())
                newState.stateStack.pop();
            }

            // Push LHS and new state
            newState.symbolStack.push(lhs);
            int newStackState = newState.stateStack.top();
            newState.stateStack.push(gotoTable[newStackState][lhs]);

            found = true;
            break;
          }
          currentRule++;
        }
        if (found)
          break;
      }
    }

    simulationStates.push_back(newState);
  }

  currentSimulationStep++;
}


void InputParsing::simulateParser()
{
  cout << "Simulation starts..." << endl;
  prepareSimulation();
  while (hasNextStep())
  {
    nextStep();
  }
  const SimulationState &finalState = simulationStates.back();

  if(finalState.accepted){
    cout << "Input Accepted " << endl;
  }else if(finalState.error){
    cout << "Input Rejected " << endl;
  }
  else{
    cout << "Parsing stop at unkown step " << endl;
  }

}
