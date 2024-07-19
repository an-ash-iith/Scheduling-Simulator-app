// const express = require('express');
// const { execFile } = require('child_process');
// const bodyParser = require('body-parser');
// const path = require('path');

// const app = express();
// app.use(bodyParser.json());

// // Serve static files from the 'public' directory
// app.use(express.static(path.join(__dirname, 'public')));

// app.post('/run_cpp', (req, res) => {
//     console.log("hi1 ");
//     const { sched, preempt, thd, args } = req.body; // Receive the array of arguments
//     console.log(req.body);
//     const cppArgs = [sched, preempt, thd, ...args];

 

//     execFile('./immediater11', cppArgs, (error, stdout, stderr) => {
//         if (error) {
//             console.error("Execution error:", error);
//             console.error("Stderr:", stderr);
//             return res.status(500).json({ error: error.message });
//         }
//         res.json({
//             stdout: stdout,
//             stderr: stderr
//         });
//     });
    
// });

// const PORT = 3000;
// app.listen(PORT, () => {
//     console.log(`Server is running on port ${PORT}`);
// });

const express = require('express');
const { execFile } = require('child_process');
const bodyParser = require('body-parser');
const path = require('path');

const app = express();
app.use(bodyParser.json());

// Serve static files from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));

app.post('/run_cpp', (req, res) => {
    console.log("Received request at /run_cpp");
    const { sched, preempt, thd, args } = req.body; // Receive the array of arguments
    console.log("Request body:", req.body);
    const cppArgs = [sched, preempt, thd, ...args];

    const pathToExecutable = path.join(__dirname, 'immediater11');
    console.log("Executing file at:", pathToExecutable);
    console.log("With arguments:", cppArgs);

    execFile(pathToExecutable, cppArgs, (error, stdout, stderr) => {
        if (error) {
            console.error("Execution error:", error);
            console.error("Stderr:", stderr);
            return res.status(500).json({ error: error.message });
        }
        console.log("Execution successful. Stdout:", stdout);
        console.log("Stderr:", stderr);
        res.json({
            stdout: stdout,
            stderr: stderr
        });
    });
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
