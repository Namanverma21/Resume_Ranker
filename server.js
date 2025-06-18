const express = require("express");
const cors = require("cors");
const multer = require("multer");
const path = require("path");
const { execFile } = require("child_process");
const fs = require("fs");

const app = express();
const PORT = 3000;

app.use(cors());
app.use(express.static("frontend"));

const storage = multer.diskStorage({
  destination: "uploads/",
  filename: (req, file, cb) => {
    const uniqueName = Date.now() + "-" + file.originalname;
    cb(null, uniqueName);
  }
});

const upload = multer({ storage: storage }).fields([
  { name: "resume", maxCount: 1 },
  { name: "jobdesc", maxCount: 1 }
]);

app.post("/upload", (req, res) => {
  upload(req, res, (err) => {
    if (err) return res.status(500).send("Upload failed: " + err.message);

    const resumePath = req.files["resume"][0].path;
    const jobdescPath = req.files["jobdesc"][0].path;

    const execPath = process.platform === "win32" ? "backend\\analyze.exe" : "./backend/analyze";
    const command = [resumePath, jobdescPath];

    execFile(execPath, command, (error, stdout, stderr) => {
      if (error) {
        console.error("Execution error:", error);
        return res.status(500).send("Error executing C++ code.");
      }

      // Save to a downloadable report
      fs.writeFileSync("frontend/analysis.txt", stdout);
      res.send(stdout);
    });
  });
});

app.listen(PORT, () => {
  console.log(`✅ Resume Ranker API is running at http://localhost:${PORT}`);
});
