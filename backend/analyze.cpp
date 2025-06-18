#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

// ✅ Expanded keyword list
vector<string> technical_keywords = {
    "python", "c++", "c", "java", "sql", "html", "css", "javascript", "typescript", "go", "rust", "php",
    "react", "angular", "vue", "node.js", "express", "django", "flask", "spring", "next.js", "laravel", "fastapi",
    "linux", "docker", "kubernetes", "git", "github", "jenkins", "terraform", "ansible", "bash", "powershell",
    "aws", "azure", "gcp", "cloud", "ci/cd", "rest", "graphql", "api",
    "machine learning", "deep learning", "data science", "pandas", "numpy", "matplotlib", "tensorflow", "keras", "scikit-learn", "opencv", "pytorch", "big data", "spark", "hadoop",
    "jira", "figma", "postman", "firebase", "mongodb", "mysql", "postgresql", "redis", "power bi", "tableau",
    "agile", "scrum", "object-oriented programming", "oop", "data structures", "algorithms", "system design"
};

// ✅ Synonyms mapping
unordered_map<string, vector<string>> synonym_map = {
    {"javascript", {"js"}},
    {"node.js", {"node", "nodejs"}},
    {"react", {"react.js", "reactjs"}},
    {"machine learning", {"ml"}},
    {"deep learning", {"dl"}},
    {"object-oriented programming", {"oop"}},
    {"mysql", {"sql"}},
    {"github", {"git"}},
    {"pytorch", {"torch"}},
    {"postgresql", {"sql"}},
    {"api", {"rest api"}}
};

// 🔡 Convert to lowercase
string toLower(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// 📄 Read file text
string readFile(const string& path) {
    ifstream file(path);
    if (!file) {
        cerr << "❌ Error: Cannot open file " << path << "\n";
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 🧹 Clean and tokenize text
unordered_set<string> tokenize(const string& text) {
    unordered_set<string> words;
    string word;
    stringstream ss(toLower(text));
    while (ss >> word) {
        word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
        if (!word.empty()) words.insert(word);
    }
    return words;
}

// 🧠 Match keywords (with synonyms)
unordered_set<string> extractKeywords(const unordered_set<string>& words) {
    unordered_set<string> found;
    for (const auto& keyword : technical_keywords) {
        string key = toLower(keyword);
        if (words.find(key) != words.end()) {
            found.insert(key);
        } else if (synonym_map.count(key)) {
            for (const auto& synonym : synonym_map[key]) {
                if (words.find(toLower(synonym)) != words.end()) {
                    found.insert(key);
                    break;
                }
            }
        }
    }
    return found;
}

// ✅ Output to terminal and file
void printAndWrite(ofstream& file, const string& text) {
    cout << text;
    file << text;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: analyze <resume_path> <jobdesc_path>\n";
        return 1;
    }

    string resumeText = readFile(argv[1]);
    string jobText = readFile(argv[2]);

    if (resumeText.empty() || jobText.empty()) {
        cerr << "❌ One of the files is empty or unreadable.\n";
        return 1;
    }

    unordered_set<string> resumeWords = tokenize(resumeText);
    unordered_set<string> jobWords = tokenize(jobText);

    unordered_set<string> resumeKeywords = extractKeywords(resumeWords);
    unordered_set<string> jobKeywords = extractKeywords(jobWords);

    ofstream report("analysis.txt");
    if (!report) {
        cerr << "❌ Could not create report file.\n";
        return 1;
    }

    printAndWrite(report, "\n🔍 Job Description Keywords:\n");
    for (const auto& kw : jobKeywords)
        printAndWrite(report, "- " + kw + "\n");

    printAndWrite(report, "\n📄 Resume Keywords:\n");
    for (const auto& kw : resumeKeywords)
        printAndWrite(report, "- " + kw + "\n");

    int matched = 0;
    vector<string> missing;

    for (const auto& keyword : jobKeywords) {
        if (resumeKeywords.find(keyword) != resumeKeywords.end()) {
            matched++;
        } else {
            missing.push_back(keyword);
        }
    }

    int total = jobKeywords.size();
    int score = total > 0 ? (matched * 100) / total : 0;

    printAndWrite(report, "\n✅ Match Score: " + to_string(score) + "%\n");

    printAndWrite(report, "\n❌ Missing Keywords:\n");
    if (missing.empty()) {
        printAndWrite(report, "None\n");
    } else {
        for (const auto& keyword : missing) {
            printAndWrite(report, "- " + keyword + "\n");
        }
    }

    printAndWrite(report, "\n💡 Suggestions to Improve Resume:\n");
    if (missing.empty()) {
        printAndWrite(report, "Great! Your resume covers all required skills.\n");
    } else {
        for (const auto& keyword : missing) {
            printAndWrite(report, "- Try to include or highlight your experience with \"" + keyword + "\" if applicable.\n");
        }
    }

    report.close();
    cout << "\n📁 Report saved to: analysis.txt\n";

    return 0;
}
