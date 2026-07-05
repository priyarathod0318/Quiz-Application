#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

class Question
{
public:
    string question;
    vector<string> options;
    int correctOption;
};

class Quiz
{
private:
    vector<Question> questions;
    int score = 0;

public:

    bool loadQuestions(string filename)
    {
        ifstream file(filename);

        if(!file)
        {
            cout << "Unable to open file.\n";
            return false;
        }

        while(true)
        {
            Question q;

            if(!getline(file, q.question))
                break;

            q.options.clear();

            for(int i=0;i<4;i++)
            {
                string option;
                getline(file, option);
                q.options.push_back(option);
            }

            string ans;
            getline(file, ans);

            q.correctOption = stoi(ans);

            questions.push_back(q);
        }

        file.close();
        return true;
    }

    void startQuiz()
    {
        cout << "\n========== QUIZ ==========\n";

        for(int i=0;i<questions.size();i++)
        {
            askQuestion(questions[i], i+1);
        }

        showResult();
    }

private:

    void askQuestion(Question &q, int number)
    {
        cout << "\n-----------------------------\n";
        cout << "Question " << number << endl;
        cout << q.question << endl;

        for(int i=0;i<4;i++)
        {
            cout << i+1 << ". " << q.options[i] << endl;
        }

        atomic<bool> answered(false);

        thread timer([&]()
        {
            for(int t=15;t>=1;t--)
            {
                if(answered)
                    return;

                cout << "\rTime Left: " << t << " sec " << flush;

                this_thread::sleep_for(chrono::seconds(1));
            }

            if(!answered)
            {
                cout << "\nTime Over!\n";
            }
        });

        int choice;

        cout << "\nEnter choice: ";
        cin >> choice;

        answered = true;

        timer.join();

        if(choice == q.correctOption)
        {
            cout << "Correct!\n";
            score++;
        }
        else
        {
            cout << "Wrong!\n";
            cout << "Correct Answer: "
                 << q.options[q.correctOption-1] << endl;
        }
    }

    void showResult()
    {
        cout << "\n==============================\n";
        cout << "Quiz Finished\n";

        cout << "Score : " << score
             << " / " << questions.size() << endl;

        double percent =
            (double)score/questions.size()*100;

        cout << "Percentage : " << percent << "%\n";

        if(percent >= 90)
            cout << "Performance : Excellent\n";
        else if(percent >= 70)
            cout << "Performance : Good\n";
        else if(percent >= 50)
            cout << "Performance : Average\n";
        else
            cout << "Performance : Needs Improvement\n";
    }
};

int main()
{
    Quiz quiz;

    if(!quiz.loadQuestions("questions.txt"))
        return 0;

    quiz.startQuiz();

    return 0;
}