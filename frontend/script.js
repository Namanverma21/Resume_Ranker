// ✅ script.js

document.getElementById("uploadForm").addEventListener("submit", async function (e) {
  e.preventDefault();

  const resumeFile = document.getElementById("resume").files[0];
  const jobdescFile = document.getElementById("jobdesc").files[0];

  if (!resumeFile || !jobdescFile) {
    alert("Please upload both files.");
    return;
  }

  const formData = new FormData();
  formData.append("resume", resumeFile);
  formData.append("jobdesc", jobdescFile);

  try {
    const response = await fetch("http://localhost:3000/upload", {
      method: "POST",
      body: formData
    });

    const resultText = await response.text();
    document.getElementById("result").innerText = resultText;

    // Add download link
    const blob = new Blob([resultText], { type: "text/plain" });
    const url = URL.createObjectURL(blob);

    const downloadBtn = document.createElement("a");
    downloadBtn.href = url;
    downloadBtn.download = "analysis.txt";
    downloadBtn.innerText = "Download Report";
    downloadBtn.className = "download-button";

    document.getElementById("result").appendChild(document.createElement("br"));
    document.getElementById("result").appendChild(downloadBtn);

    // Clear form
    document.getElementById("uploadForm").reset();
  } catch (error) {
    document.getElementById("result").innerText = "Something went wrong!";
    console.error("Error:", error);
  }
});
