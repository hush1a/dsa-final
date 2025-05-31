#include <iostream>
using namespace std;

// --------------------- Topic and Course Structures ---------------------

// Represents a topic in a course
struct TopicNode {
    string topic;
    TopicNode* next;
};

// Represents a course, which has a name and a linked list of topics
struct CourseNode {
    string courseName;
    TopicNode* topicList;
    CourseNode* next;
};

// Head pointer to the list of all courses
CourseNode* courseHead = nullptr;

// --------------------- Student Progress Structures ---------------------

// Represents a completed topic by a student
struct CompletedTopicNode {
    string topic;
    CompletedTopicNode* next;
};

// Represents a student, with a list of completed topics
struct StudentNode {
    string studentId;
    CompletedTopicNode* completedTopics;
    StudentNode* next;
};

// Head pointer to the list of all students
StudentNode* studentHead = nullptr;

// --------------------- Submission and Review Structures ---------------------

// Submission structure: stores studentId, content, and current review level
struct Submission {
    string studentId;
    string content;
    int level; // 0 = first, 1 = mid, 2 = final
};

// Linked list node for the submission queue
struct SubmissionNode {
    Submission data;
    SubmissionNode* next;
};

// Front and rear pointers for the submission queue
SubmissionNode* reviewFront = nullptr;
SubmissionNode* reviewRear = nullptr;

// --------------------- Rollback Stack ---------------------

// Stores submission state and its status for rollback
struct SubmissionState {
    Submission submission;
    string status;
};

// Node for the rollback stack (LIFO)
struct SubmissionStateNode {
    SubmissionState data;
    SubmissionStateNode* next;
};

// Top pointer for rollback stack
SubmissionStateNode* rollbackTop = nullptr;

// --------------------- Utility Functions ---------------------

// Finds a course by name in the course list
CourseNode* findCourse(string courseName) {
    CourseNode* current = courseHead;
    while (current) {
        if (current->courseName == courseName) return current;
        current = current->next;
    }
    return nullptr;
}

// Finds a student by ID in the student list
StudentNode* findStudent(string studentId) {
    StudentNode* current = studentHead;
    while (current) {
        if (current->studentId == studentId) return current;
        current = current->next;
    }
    return nullptr;
}

// --------------------- Core Functionalities ---------------------

// Adds a topic to a course (if course doesn't exist, create it)
void addContent(string course, string topic) {
    CourseNode* c = findCourse(course);
    if (!c) {
        // Create new course and add to front of course list
        c = new CourseNode{course, nullptr, courseHead};
        courseHead = c;
    }

    // Add topic to front of course's topic list
    TopicNode* newTopic = new TopicNode{topic, c->topicList};
    c->topicList = newTopic;
}

// Marks a topic as completed for a student (creates student if not found)
void completeTopic(string studentId, string topic) {
    StudentNode* s = findStudent(studentId);
    if (!s) {
        // Create new student and add to front of student list
        s = new StudentNode{studentId, nullptr, studentHead};
        studentHead = s;
    }

    // Check if topic is already completed
    CompletedTopicNode* temp = s->completedTopics;
    while (temp) {
        if (temp->topic == topic) return; // already completed
        temp = temp->next;
    }

    // Add topic to front of completed topics list
    CompletedTopicNode* newTopic = new CompletedTopicNode{topic, s->completedTopics};
    s->completedTopics = newTopic;
}

// Displays a student's completed topics
void viewProgress(string studentId) {
    StudentNode* s = findStudent(studentId);
    if (!s) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Progress of " << studentId << ": ";
    CompletedTopicNode* temp = s->completedTopics;
    while (temp) {
        cout << temp->topic << " -> ";
        temp = temp->next;
    }
    cout << "NULL\n";
}

// Submits an assignment (enqueues to review queue and pushes to rollback stack)
void submitAssignment(string studentId, string content) {
    Submission s = {studentId, content, 0};
    SubmissionNode* newNode = new SubmissionNode{s, nullptr};

    // Enqueue to review queue
    if (!reviewFront) {
        reviewFront = reviewRear = newNode;
    } else {
        reviewRear->next = newNode;
        reviewRear = newNode;
    }

    // Push to rollback stack
    SubmissionStateNode* stateNode = new SubmissionStateNode{{s, "submitted"}, rollbackTop};
    rollbackTop = stateNode;
}

// Reviews the next assignment in the queue
void reviewStep() {
    if (!reviewFront) {
        cout << "No submissions to review.\n";
        return;
    }

    // Dequeue the front submission
    SubmissionNode* node = reviewFront;
    Submission s = node->data;
    reviewFront = reviewFront->next;
    if (!reviewFront) reviewRear = nullptr; // reset rear if queue is empty
    delete node;

    cout << "Reviewing submission by " << s.studentId << " at level " << s.level << "\n";

    if (s.level < 2) {
        // Not yet final, re-enqueue with next level
        s.level++;
        submitAssignment(s.studentId, s.content);

        // Update top of rollback stack to new status and level
        rollbackTop->data.submission.level = s.level;
        rollbackTop->data.status = "reviewed";
    } else {
        // Finalized review, push to rollback stack as finalized
        SubmissionStateNode* stateNode = new SubmissionStateNode{{s, "finalized"}, rollbackTop};
        rollbackTop = stateNode;
    }
}

// Undoes the last submission action
void rollback() {
    if (!rollbackTop) {
        cout << "Nothing to rollback.\n";
        return;
    }

    SubmissionStateNode* temp = rollbackTop;
    rollbackTop = rollbackTop->next;

    cout << "Rolled back submission by " << temp->data.submission.studentId
         << " to status: " << temp->data.status << "\n";

    delete temp;
}

// --------------------- Demo Main ---------------------

int main() {
    // Add course content
    addContent("CS101", "Intro to CS");
    addContent("CS101", "Linked Lists");

    // Mark topics as completed
    completeTopic("bayu01", "Intro to CS");
    completeTopic("bayu01", "Linked Lists");

    // View student progress
    viewProgress("bayu01");

    // Simulate a submission and review process
    submitAssignment("bayu01", "Assignment 1");
    reviewStep(); // Goes to level 1
    reviewStep(); // Goes to level 2
    rollback();   // Roll back final review

    return 0;
}
