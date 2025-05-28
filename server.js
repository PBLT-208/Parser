const express = require('express');
const fs = require('fs');
const { exec } = require('child_process');
const path = require('path');

const app = express();
const PORT = 3000;

app.use(express.static('.')); // serve index.html
app.use(express.json());

app.post('/run-parser', (req, res) => {
  const { grammar, input } = req.body;

  // Write to files
  fs.writeFileSync('grammarInput.txt', grammar);
  fs.writeFileSync('input.txt', input);

  // Run the compiled C++ program (make sure executable is built beforehand)
  exec('parser.exe', (error, stdout, stderr) => {
    if (error) {
      return res.send(`Execution Error:\n${stderr || error.message}`);
    }
    res.send(stdout);
  });
});

app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});
