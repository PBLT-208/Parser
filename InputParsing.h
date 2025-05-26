#include <bits/stdc++.h>
using namespace std;

struct SimulationState
{
  stack<int> stateStack;
  stack<string> symbolStack;
  size_t inputPointer;
  string currentAction;
  bool accepted;
  bool error;
};

class InputParsing
{
  vector<SimulationState> simulationStates;
  size_t currentSimulationStep;
  map<int, map<string, string>> actionTable;
  map<int, map<string, int>> gotoTable;
  map<string, vector<vector<string>>> augment_prod;

public:
  InputParsing(map<int, map<string, string>> actionTable,
               map<int, map<string, int>> gotoTable, map<string, vector<vector<string>>> augment_prod);
  void prepareSimulation();
  bool hasNextStep() const;
  void nextStep();
  void simulateParser();
  string getCurrentStepOutput();
};