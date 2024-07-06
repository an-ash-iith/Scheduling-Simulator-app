// const express = require('express');
// const { execFile } = require('child_process');
// const bodyParser = require('body-parser');
// const path = require('path');

// const app = express();
// app.use(bodyParser.json());

// // Serve static files from the 'public' directory
// app.use(express.static(path.join(__dirname, 'public')));

// app.post('/run_cpp', (req, res) => {

    
//     const {sched,preempt,thd, args } = req.body; // Receive the array of arguments
//     const cppArgs = [sched, preempt, thd, ...args];
//     execFile('./immediater', cppArgs, (error, stdout, stderr) => {
//         if (error) {
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
    const { sched, preempt, thd, args } = req.body; // Receive the array of arguments
    const cppArgs = [sched, preempt, thd, ...args];

    execFile('./immediater', cppArgs, { maxBuffer: 1024 * 500 }, (error, stdout, stderr) => {
        if (error) {
            return res.status(500).json({ error: error.message });
        }

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
