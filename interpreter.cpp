#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

/* �������� ����� */
int nextToken;
int charClass;
char lexeme[100];
char nextChar;

int lexLen;
int token;

string save[100];
string temp[100];
int token_save[100];
int save_index = 0;
int index_ = 0;

/* �Լ� ����� */

void addChar();
void getChar();
void getNonBlank();
int lex();
void reserved();
float add();
float mul();
string setq();
float sub();
float div();
string* list();
string car();
string* cdr();
string caddr();
string nth();

bool atom();
bool null();
bool numberp();
bool isINTorFLOAT(string a); //check if it is INT / FLOAT 
bool zerop();
bool minusp();
bool equal();
bool gt();
bool ge();
bool lt();
bool le();
bool isfloat(string a); //check just if it is FLOAT
void cons();
void reverse();
void appendlist();
void length();
string* member();

bool stringp();
string assoc();
void remove();
void subst();
void print();
bool iif();
bool cond();
bool tempmain();


#define LETTER 0
#define DIGIT 1
#define END -1
#define UNKNOWN 99
#define SEMICOLON 27

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMICOLON 27
#define COMP_OP_GT 29
#define COMP_OP_LT 53 
#define DOT 55
#define LEFT_BRACKET 45
#define RIGHT_BRACKET 46

/* reserved words */
#define SETQ 30
#define LIST 31
#define CAR 32
#define CDR 33
#define CADDR 34        
#define CONS 35
#define REVERSE 36
#define APPEND 37
#define LENGTH 38
#define ATOM 39
#define NULLP 40
#define NUMBERP 41
#define ZEROP 42
#define MINUSP 43
#define EQUAL 44
#define NTH 47
#define STRINGP 48
#define MEMBER 49
#define ASSOC 50 
#define REMOVE 51
#define SUBST 52
#define IF 56
#define COND 57
#define NIL 58
#define PRINT 59


/* SYMBOL �� LIST ���� ���Ḯ��Ʈ ��� ���� */
typedef struct lisp_list {
    string name;
    string data;
    lisp_list* includeNode; //����Ʈ�� �� Ȱ��
    lisp_list* nextNode; //���� ��� ������
}LISP_list;

LISP_list* head = NULL;

/* ���� �������� ��带 Ž���ϴ� �Լ� */
string findNode_horizon_data(string varname) {

    if (head == NULL) {      //�ƹ��� ��嵵 �������� ���� ��
        return "NotExist";
    }
    else { //��尡 ������ ��
        LISP_list* temp = head;


        while (1) {
            if (!((temp->name).compare(varname)))
                break;
            else if (temp->nextNode == NULL) {
                return "NotExist";
            }

            temp = temp->nextNode;
        }

        if (temp->includeNode != NULL) { //symbol�� �ƴ� list ������ �� ��
            return "LISTDATA";
        }
        else {
            return temp->data;
        }

    }

}
LISP_list* findNode_horizon_addr(string varname) { //�̹� varname�� ��尡 list���� Ȯ������ ���� ������
    if (head == NULL) {
        return NULL;
    }
    else {
        LISP_list* temp = head;


        while (1) { //����Ʈ  Ž��.
            if (!((temp->name).compare(varname)))
                break;
            else if (temp->nextNode == NULL) {
                cout << varname << " doesn't exist" << endl;
                return NULL;
            }

            temp = temp->nextNode;
        }

        return temp;
    }

}

void append(string name, string data, int isList) { // isList�� true�̸� List�̰� �ƴϸ� symbol��.
    LISP_list* temp;
    if (!isList) { //symbol�� ��
        if (head == NULL) { //�ƹ��� ��尡 �������� ���� ��
            LISP_list* node = new LISP_list;
            node->name = name;
            node->data = data;
            node->includeNode = NULL;
            node->nextNode = NULL;

            head = node;

        }
        else { //�����ϴ� ��尡 ���� ��
            int checkSymbolName = 0;
            temp = head;

            while (temp != NULL) {
                if (!(temp->name).compare(name)) {
                    checkSymbolName = 1;
                    break;
                }

                temp = temp->nextNode;
            }

            if (checkSymbolName == 1) {
                temp->data = data; //�̹� ����� value update
            }
            else {
                temp = head;

                while (temp->nextNode != NULL) {
                    temp = temp->nextNode;
                }
                LISP_list* node = new LISP_list;
                node->name = name;
                node->data = data;
                node->includeNode = NULL;
                node->nextNode = NULL;

                temp->nextNode = node;
            }
        }
    }
    else {  //����Ʈ�� ��
        if (head == NULL) { //�ƹ��� ��� ���� ��
            LISP_list* node = new LISP_list;
            node->name = name;
            node->data = data;
            node->includeNode = NULL;
            node->nextNode = NULL;

            head = node;

        }
        else { //��� ���� ��
            temp = head;

            int checkListname = 0;

            while (temp != NULL) {
                if (!(temp->name).compare(name)) {
                    checkListname = 1;
                    break;
                }

                temp = temp->nextNode;
            }

            if (checkListname == 0) {  // case 1: List name does not exist in LL, create Node
                temp = head;
                while (temp->nextNode != NULL) {
                    temp = temp->nextNode;
                }
                LISP_list* node = new LISP_list;
                node->name = name;
                node->data = data;
                node->includeNode = NULL;
                node->nextNode = NULL;

                temp->nextNode = node;

            }
            else { //  case 2: List name already exists in Linked List //

                while (temp->includeNode != NULL) {
                    temp = temp->includeNode;
                }

                LISP_list* node = new LISP_list;
                node->name = name;
                node->data = data;
                node->includeNode = NULL;
                node->nextNode = NULL;

                temp->includeNode = node;

            }

        }


    }
}

/* main   */

int main() {
    while (1) {
        cout << ">> ";

        getChar(); //lexcial analysis 
        do {
            lex();
        } while (nextToken != SEMICOLON);

        save_index--;  //delete semicolon count
        for (index_ = 0; index_ < save_index; index_++) {
            if (!(save[index_].compare("+"))) // ���� ����
            {
                cout << add() << endl;
            }
            else if (!(save[index_].compare("*"))) // ���� ����
            {
                cout << mul() << endl;
            }
            else if (!(save[index_].compare("-"))) // ���� ����
            {
                cout << sub() << endl;
            }
            else if (!(save[index_].compare("/"))) // ������ ����
            {
                cout << div() << endl;
            }
            else if (!(save[index_].compare("SETQ")) || !(save[index_].compare("setq"))) { // SETQ ����
                string temp = setq();
                string printdata;
                if ((findNode_horizon_data(temp).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                    LISP_list* checkPoint = findNode_horizon_addr(temp);

                    while (1) {
                        printdata += checkPoint->data;
                        printdata += " ";

                        if (checkPoint->includeNode == NULL)
                            break;

                        checkPoint = checkPoint->includeNode;
                    }
                    stringstream ss(printdata);
                    string k;
                    cout << "(";
                    ss >> k;
                    cout << k;
                    while (ss >> k) {
                        cout << " " << k;

                    }
                    cout << ")" << endl;

                }
                else { //�����ϴ� ����� SYMBOL
                    printdata = findNode_horizon_data(temp);
                    cout << printdata << endl;
                }
            }
            else if (!(save[index_].compare("LIST")) || !(save[index_].compare("list"))) {
                string* array;
                array = list();
                int arrayindex = 0;
                while (!array[arrayindex].empty()) //list ���� count
                    arrayindex++;

                if (arrayindex >= 1) {

                    cout << "(";
                    for (int i = 0; i < arrayindex; i++) {


                        if (i == arrayindex - 1)
                            cout << array[i];
                        else
                            cout << array[i] << " ";

                    }
                    cout << ")" << endl;

                }

            }

            else if (!(save[index_].compare("CAR")) || !(save[index_].compare("car"))) {
                string str;
                index_++;
                str = car();

                if (str.length() > 1 && str[0] == '(') {  //����Ʈ  �� ����Ʈ ���� ����� ���
                    stringstream ss(str);
                    string k;
                    if (ss >> k)
                        cout << k;

                    if (ss >> k)
                        cout << k;
                    while (ss >> k) {
                        if (k == ")")
                            cout << k << endl;
                        else
                            cout << " " << k;
                    }


                }
                else {
                    cout << str << endl; //�ܼ� ����Ʈ ��� �� ��
                }

            }
            else if (!(save[index_].compare("CDR")) || !(save[index_].compare("cdr"))) {
                index_++;
                string* list_array = cdr();

                int arrayindex = 0;
                while (!list_array[arrayindex].empty()) //cdr ������ ����Ʈ ���� count
                    arrayindex++;

                if (arrayindex >= 1) {
                    cout << "(";
                    for (int i = 0; i < arrayindex; i++) {

                        if (list_array[i].length() > 1 && list_array[i][0] == '(') { //����Ʈ ��ø�� ���

                            stringstream ss(list_array[i]);
                            string k;
                            if (ss >> k)
                                cout << k;

                            if (ss >> k)
                                cout << k;
                            while (ss >> k) {
                                if (k == ")") {
                                    if (i != arrayindex - 1)
                                        cout << k << " ";
                                    else
                                        cout << k;
                                }
                                else
                                    cout << " " << k;
                            }

                        }
                        else if (i == arrayindex - 1) { //��� ���� ���߱� ����
                            cout << list_array[i];
                        }
                        else {
                            cout << list_array[i] << " ";
                        }

                    }
                    cout << ")" << endl;
                }

            }
            else if (!(save[index_].compare("CADDR")) || !(save[index_].compare("caddr"))) {
                string str;
                index_++;
                str = caddr();

                if (str.length() > 1 && str[0] == '(') { // list����

                    for (int i = 0; i < str.length(); i++) {


                        if ((i == 0) || (i == str.length() - 2) || (i == str.length() - 1))
                            cout << str.substr(i, 1);
                        else
                            cout << str.substr(i, 1) << " ";


                    }
                    cout << endl;

                }
                else {
                    cout << str << endl;
                }

            }
            else if (!(save[index_].compare("CONS")) || !(save[index_].compare("cons"))) //cons ����
            {
                cons();
            }
            else if (!(save[index_].compare("REVERSE")) || !(save[index_].compare("reverse"))) //reverse����
            {
                reverse();
            }
            else if (!(save[index_].compare("APPEND")) || !(save[index_].compare("append"))) //append����
            {
                appendlist();
            }
            else if (!(save[index_].compare("LENGTH")) || !(save[index_].compare("length"))) //length ����
            {
                length();
            }
            else if (!(save[index_].compare("ATOM")) || !(save[index_].compare("atom"))) { //atom ����
                bool temp = atom();
                if (temp == true)
                    cout << "true" << endl;
                else
                    cout << "false" << endl;

            }
            else if (!(save[index_].compare("NULL")) || !(save[index_].compare("null"))) { //null�Լ� ����
                bool temp = null();
                if (temp == true)
                    cout << "true" << endl;
                else
                    cout << "false" << endl;
            }
            else if (!(save[index_].compare("NUMBERP")) || !(save[index_].compare("numberp"))) { //numberp �Լ� ����
                bool temp = numberp();
                if (temp == true)
                    cout << "true" << endl;
                else
                    cout << "false" << endl;
            }

            else if (!(save[index_].compare("ZEROP")) || !(save[index_].compare("zerop"))) { //zerop �Լ� ����
                bool temp = zerop();
                if (temp == true)
                    cout << "true" << endl;
                else
                    cout << "false" << endl;
            }
            else if (!(save[index_].compare("MINUSP")) || !(save[index_].compare("minusp"))) { //minusp ����
                bool temp = minusp();
                if (temp == true)
                    cout << "true" << endl;
                else
                    cout << "false" << endl;

                index_ = save_index - 1;
            }
            else if (!(save[index_].compare("EQUAL")) || !(save[index_].compare("equal")) || save[index_] == "=") { //equal ����

                bool temp = equal();
                if (temp == true)
                    cout << "true" << endl;
                else
                    cout << "false" << endl;

                index_ = save_index - 1;
            }
            else if (!(save[index_].compare(">"))) {
                if (!(save[index_ + 1].compare("="))) {
                    bool temp = ge();
                    if (temp == true)
                        cout << "true" << endl;
                    else
                        cout << "false" << endl;
                }
                else {
                    bool temp = gt();
                    if (temp == true)
                        cout << "true" << endl;
                    else
                        cout << "false" << endl;
                }
            }
            else if (!(save[index_].compare("<"))) {
                if (!(save[index_ + 1].compare("="))) {
                    bool temp = le();
                    if (temp == true)
                        cout << "true" << endl;
                    else
                        cout << "false" << endl;
                }
                else {
                    bool temp = lt();
                    if (temp == true)
                        cout << "true" << endl;
                    else
                        cout << "false" << endl;
                }
            }

            else if (!(save[index_].compare("NTH")) || !(save[index_].compare("nth"))) {
                string temp = nth();

                cout << temp << endl;

                index_ = save_index - 1;
            }

            else if (!(save[index_].compare("STRINGP")) || !(save[index_].compare("stringp"))) {
                bool temp = stringp();
                if (temp == true)
                    cout << "true" << endl;
                else
                    cout << "false" << endl;

                index_ = save_index - 1;
            }
            else if (!(save[index_].compare("MEMBER")) || !(save[index_].compare("member"))) {
                string* list_array = member();
                int arrayindex = 0;
                while (!list_array[arrayindex].empty())
                    arrayindex++;

                if (arrayindex >= 1) {
                    cout << "(";
                    for (int i = 0; i < arrayindex; i++) {

                        if (list_array[i].length() > 1 && list_array[i][0] == '(') {

                            for (int j = 0; j < list_array[i].length(); j++) {


                                if ((j == 0) || (j == list_array[i].length() - 2))
                                    cout << list_array[i].substr(j, 1);
                                else if ((j == list_array[i].length() - 1)) {
                                    if (i == arrayindex - 1)
                                        cout << list_array[i].substr(j, 1);
                                    else
                                        cout << list_array[i].substr(j, 1) << " ";
                                }

                                else
                                    cout << list_array[i].substr(j, 1) << " ";


                            }
                        }
                        else if (i == arrayindex - 1) {
                            cout << list_array[i];
                        }
                        else {
                            cout << list_array[i] << " ";
                        }

                    }
                    cout << ")" << endl;
                }
                index_ = save_index - 1;
            }
            else if (!(save[index_].compare("ASSOC")) || !(save[index_].compare("assoc"))) {
                string temp = assoc();

                stringstream ss(temp);
                string k;
                if (ss >> k)
                    cout << k;

                if (ss >> k)
                    cout << k;
                while (ss >> k) {
                    if (k == ")")
                        cout << k << endl;
                    else
                        cout << " " << k;


                }

                index_ = save_index - 1;
            }
            else if (!(save[index_].compare("REMOVE")) || !(save[index_].compare("remove"))) {
                remove();
                index_ = save_index - 1;
            }
            else if (!(save[index_].compare("SUBST")) || !(save[index_].compare("subst"))) {
                subst();

                index_ = save_index - 1;
            }
            else if (!(save[index_].compare("IF")) || !(save[index_].compare("if"))) {
                iif();
                index_ = save_index - 1;
            }
            else if (!(save[index_].compare("COND")) || !(save[index_].compare("cond"))) {
                cond();
                index_ = save_index - 1;
            }
            else if (!(save[index_].compare("PRINT")) || !(save[index_].compare("print"))) {
                print();
            }

            else if (token_save[index_] == IDENT) {
                string temp;
                if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                    LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                    while (1) {
                        temp += checkPoint->data;
                        temp += " ";

                        if (checkPoint->includeNode == NULL)
                            break;

                        checkPoint = checkPoint->includeNode;
                    }
                    stringstream ss(temp);
                    string k;
                    cout << "(";
                    ss >> k;
                    cout << k;
                    while (ss >> k) {
                        cout << " " << k;

                    }
                    cout << ")" << endl;


                }
                else if (findNode_horizon_data(save[index_]) == "NotExist") {
                    cout << save[index_] << " doesn't exist" << endl;

                }
                else {
                    cout << findNode_horizon_data(save[index_]) << endl; // list ��µ� �ؾ���
                }
            }

        }

        save_index = 0;

    }
    return 0;
}

/******************************************
* lookup - a function to lookup operators
* and parentheses and return the token
******************************************/
int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '=':
        addChar();
        nextToken = ASSIGN_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    case '<':
        addChar();
        nextToken = COMP_OP_LT;
        break;
    case '>':
        addChar();
        nextToken = COMP_OP_GT;
        break;
    case '{':
        addChar();
        nextToken = LEFT_BRACKET;
        break;
    case '}':
        addChar();
        nextToken = RIGHT_BRACKET;
        break;
    case '.':
        addChar();
        nextToken = DOT;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

/**************************************************/
/* addChar - a function to add nextChar to lexeme */
/**************************************************/
void addChar() {
    if (lexLen <= 98) {  // max length of Lexime is 99
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0; // '\0'
    }
    else {
        cout << "Error - lexeme is too long";
    }
}

/*****************************************************/
/* getChar - a function to get the next character
of input and determine its character class */
/*****************************************************/
void getChar() {
    if ((nextChar = getc(stdin))) {

        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar) || nextChar == '-')
            charClass = DIGIT;
        else if (nextChar == ';')
            charClass = SEMICOLON;
        else
            charClass = UNKNOWN;
    }

}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character        */
/*****************************************************/
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
expressions                                 */
/*****************************************************/
int lex() {
    lexLen = 0;
    getNonBlank();
    int count = 0;
    switch (charClass) {
        /* Parse identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        reserved();
        break;

        /* Parse integer literals */
    case DIGIT:
        addChar();
        getChar();


        while (charClass == DIGIT || nextChar == '.') {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;

        count++;
        break;

    case SEMICOLON:
        nextToken = SEMICOLON;
        addChar();
        getChar();
        break;
        /* Parentheses and operators */
    case UNKNOWN:

        lookup(nextChar);
        getChar();
        break;

        /* EOF */
    case END:
        nextToken = END;
        break;
    } /* End of switch */

    save[save_index] = lexeme;
    token_save[save_index] = nextToken;

    // printf("Next token is: %d, Next lexeme is %s\n\n", nextToken, lexeme);
    // cout << save[save_index] << " " << token_save[save_index] << endl;
    save_index++;

    return nextToken;
} /* End of function lex */

void reserved()
{
    //����� lex�� ���� �Լ�
    string reserved[46];
    string temp;
    reserved[0] = "SETQ";
    reserved[1] = "LIST";
    reserved[2] = "CAR";
    reserved[3] = "CDR";
    reserved[4] = "CADDR";
    reserved[5] = "CONS";
    reserved[6] = "REVERSE";
    reserved[7] = "APPEND";
    reserved[8] = "LENGTH";

    reserved[9] = "ATOM";
    reserved[10] = "NULL";
    reserved[11] = "NUMBERP";
    reserved[12] = "ZEROP";
    reserved[13] = "MINUSP";
    reserved[14] = "EQUAL";
    reserved[15] = "STRINGP";
    reserved[16] = "MEMBER";
    reserved[17] = "ASSOC";
    reserved[18] = "REMOVE";
    reserved[19] = "IF";
    reserved[20] = "COND";
    reserved[21] = "NIL";
    reserved[22] = "PRINT";

    reserved[23] = "setq";
    reserved[24] = "list";
    reserved[25] = "car";
    reserved[26] = "cdr";
    reserved[27] = "caddr";
    reserved[28] = "cons";
    reserved[29] = "reverse";
    reserved[30] = "append";
    reserved[31] = "length";

    reserved[32] = "atom";
    reserved[33] = "null";
    reserved[34] = "numberp";
    reserved[35] = "zerop";
    reserved[36] = "minusp";
    reserved[37] = "equal";
    reserved[38] = "stringp";
    reserved[39] = "member";
    reserved[40] = "assoc";
    reserved[41] = "remove";
    reserved[42] = "if";
    reserved[43] = "cond";
    reserved[44] = "nil";
    reserved[45] = "print";

    for (int i = 0; i < 46; i++)
    {
        if (lexLen == reserved[i].length())
        {
            for (int y = 0; y < lexLen; y++)
            {
                if (lexeme[y] != reserved[i][y])
                    break;
                if (y == lexLen - 1)
                    switch (i) {
                    case 0:
                    case 23:
                        nextToken = SETQ;
                        break;
                    case 1:
                    case 24:
                        nextToken = LIST;
                        break;
                    case 2:
                    case 25:
                        nextToken = CAR;
                        break;
                    case 3:
                    case 26:
                        nextToken = CDR;
                        break;
                    case 4:
                    case 27:
                        nextToken = CADDR;
                        break;
                    case 5:
                    case 28:
                        nextToken = CONS;
                        break;
                    case 6:
                    case 29:
                        nextToken = REVERSE;
                        break;
                    case 7:
                    case 30:
                        nextToken = APPEND;
                        break;
                    case 8:
                    case 31:
                        nextToken = LENGTH;
                        break;
                    case 9:
                    case 32:
                        nextToken = ATOM;
                        break;
                    case 10:
                    case 33:
                        nextToken = NULLP;
                        break;
                    case 11:
                    case 34:
                        nextToken = NUMBERP;
                        break;
                    case 12:
                    case 35:
                        nextToken = ZEROP;
                        break;
                    case 13:
                    case 36:
                        nextToken = MINUSP;
                        break;
                    case 14:
                    case 37:
                        nextToken = EQUAL;
                        break;
                    case 15:
                    case 38:
                        nextToken = STRINGP;
                        break;
                    case 16:
                    case 39:
                        nextToken = MEMBER;
                        break;
                    case 17:
                    case 40:
                        nextToken = ASSOC;
                        break;
                    case 18:
                    case 41:
                        nextToken = REMOVE;
                        break;
                    case 19:
                    case 42:
                        nextToken = IF;
                        break;
                    case 20:
                    case 43:
                        nextToken = COND;
                        break;
                    case 21:
                    case 44:
                        nextToken = NIL;
                        break;
                    case 22:
                    case 45:
                        nextToken = PRINT;
                        break;
                    }
            }
        }
    }
}
bool isfloat(string a) { //�Ҽ����� �ƴ��� ���θ� ��ȯ�ϴ� �Լ�
    const char* temp = a.c_str();

    bool value = false;

    int dotCount = 0;
    for (int i = 0; temp[i] != '\0'; i++) {

        if (i == 0) { //ù��° ���ڴ� -�̰ų� ���ڰ� �;���.
            if ((temp[i] == '-') || ('0' <= temp[i] && temp[i] <= '9'))
                value = true;
            else {
                value = false;
                break;
            }
        }
        else { //ù��° ���ڰ��ƴϸ� ���ڳ� �Ҽ����� �;��� , �� �Ҽ����� �Ѱ��� �� �� ����
            if (('0' <= temp[i] && temp[i] <= '9')) {
                value = true;
            }
            else if ((temp[i] == '.')) {
                dotCount++;
                if (dotCount > 1) {
                    value = false;
                    break;
                }
                else
                    value = true;
            }
            else {
                value = false;
                break;
            }
        }



    }

    if (dotCount != 1)
        value = false;
    else
        value = true;

    return value;
}
float add() {
    // string aaa= "4.3313";
    // cout << stof(aaa);
    index_++;
    float temp = 0;
    float return_ = 0;
    for (; index_ < save_index; index_++) {
        if (!(save[index_].compare("*"))) { // add �Լ� �ȿ��� ���� �����ڸ� ������ ��
            return_ = mul();
            temp += return_;
            continue;
        }
        else if (!(save[index_].compare("+"))) { // add �Լ� �ȿ��� ���� �����ڸ� ������ ��
            return_ = add();
            temp += return_;
            continue;
        }
        else if (!(save[index_].compare("-"))) { // add �Լ� �ȿ��� ���� �����ڸ� ������ ��
            return_ = sub();
            temp += return_;
            continue;
        }
        else if (!(save[index_].compare("/"))) { // add �Լ� �ȿ��� ������ �����ڸ� ������ ��
            return_ = div();
            temp += return_;
            continue;
        }
        else if (!(save[index_].compare("("))) { // ( ������ ���� �׳� ����ħ
            continue;
        }
        else if (!(save[index_].compare(")"))) { // ) ������ ���� �Լ� ����
            break;
        }
        else if (token_save[index_] == IDENT) {
            if (findNode_horizon_data(save[index_]).compare("NotExist")) {


                if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� LIST�� �ƴ�
                    if (isfloat(findNode_horizon_data(save[index_]))) {

                        return_ = stof(findNode_horizon_data(save[index_]));
                        temp += return_;
                    }
                    else {
                        return_ = stoi(findNode_horizon_data(save[index_]));
                        temp += return_;
                    }
                }

            }
            else {
                cout << "�ش� ������ �������� �ʽ��ϴ�." << endl;
            }

        }
        else if (token_save[index_] == INT_LIT) { // �ǿ����ڸ� ������ ��
            if (isfloat(save[index_])) {
                return_ = stof(save[index_]);
                temp += return_;
            }
            else {
                temp += stoi(save[index_]);
            }
        }
    }

    return temp;
}
float mul() {

    index_++;
    float temp = 1;
    float return_ = 0;
    for (; index_ < save_index; index_++) {
        if (!(save[index_].compare("*"))) { // mul �Լ� �ȿ��� ���� �����ڸ� ������ ��
            return_ = mul();
            temp *= return_;
            continue;
        }
        if (!(save[index_].compare("+"))) { // mul �Լ� �ȿ��� ���� �����ڸ� ������ ��
            return_ = add();
            temp *= return_;
            continue;
        }
        if (!(save[index_].compare("-"))) { // mul �Լ� �ȿ��� ���� �����ڸ� ������ ��
            return_ = sub();
            temp *= return_;
            continue;
        }
        if (!(save[index_].compare("/"))) { // mul �Լ� �ȿ��� ������ �����ڸ� ������ ��
            return_ = div();
            temp *= return_;
            continue;
        }
        else if (!(save[index_].compare("("))) { // ( ������ ���� �׳� ����ħ
            continue;
        }
        else if (!(save[index_].compare(")"))) { // ) ������ ���� �Լ� ����
            break;
        }
        else if (token_save[index_] == IDENT) {
            if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� LIST�� �ƴ�
                    if (isfloat(findNode_horizon_data(save[index_]))) {
                        return_ = stof(findNode_horizon_data(save[index_]));
                        temp *= return_;
                    }
                    else {
                        return_ = stoi(findNode_horizon_data(save[index_]));
                        temp *= return_;
                    }
                }

            }
        }
        else if (token_save[index_] == INT_LIT) { // �ǿ����ڸ� ������ ��
            if (isfloat(save[index_])) {
                return_ = stof(save[index_]);
                temp *= return_;
            }
            else {
                temp *= stoi(save[index_]);
            }
        }
    }
    return temp;
}
float sub() {

    index_++;
    float temp = 0;
    int operator_num = 0;
    float return_ = 0;
    for (; index_ < save_index; index_++) {
        if (!(save[index_].compare("*")) && operator_num == 1) { // sub �Լ� �ȿ��� ���� �����ڸ� ������ �ι�° �ǿ����ڸ� ������ ��
            return_ = mul();
            temp -= return_;
            continue;
        }
        else if (!(save[index_].compare("*")) && operator_num == 0) { // sub �Լ� �ȿ��� ���� �����ڸ� ������ ù��° �ǿ����ڸ� ������ ��
            operator_num++;
            return_ = mul();
            temp += return_;
            continue;
        }
        else if (!(save[index_].compare("+")) && operator_num == 1) { // sub �Լ� �ȿ��� ���� �����ڸ� ������ �ι�° �ǿ����ڸ� ������ ��
            return_ = add();
            temp -= return_;
            continue;
        }
        else if (!(save[index_].compare("+")) && operator_num == 0) { // sub �Լ� �ȿ��� ���� �����ڸ� ������ ù��° �ǿ����ڸ� ������ ��
            operator_num++;
            return_ = add();
            temp += return_;
            continue;
        }
        else if (!(save[index_].compare("-")) && operator_num == 1) { // sub �Լ� �ȿ��� ���� �����ڸ� ������ �ι�° �ǿ����ڸ� ������ ��
            return_ = sub();
            temp -= return_;
            continue;
        }
        else if (!(save[index_].compare("-")) && operator_num == 0) { // sub �Լ� �ȿ��� ���� �����ڸ� ������ ù��° �ǿ����ڸ� ������ ��
            operator_num++;
            return_ = sub();
            temp += return_;
            continue;
        }
        else if (!(save[index_].compare("/")) && operator_num == 1) { // sub �Լ� �ȿ��� ������ �����ڸ� ������ �ι�° �ǿ����ڸ� ������ ��
            return_ = div();
            temp -= return_;
            continue;
        }
        else if (!(save[index_].compare("/")) && operator_num == 0) { // sub �Լ� �ȿ��� ������ �����ڸ� ������ ù��° �ǿ����ڸ� ������ ��
            operator_num++;
            return_ = div();
            temp += return_;
            continue;
        }
        else if (!(save[index_].compare("("))) { // ( ������ ���� �׳� ����ħ
            continue;
        }
        else if (!(save[index_].compare(")"))) { // ) ������ ���� �Լ� ����
            break;
        }
        else if ((token_save[index_] == INT_LIT) && operator_num == 0) { // ù��° �ǿ����ڸ� ������ ��
            if (isfloat(save[index_])) {
                return_ = stof(save[index_]);
                temp += return_;
            }
            else {
                temp += stoi(save[index_]);
            }
            operator_num++;
            continue;
        }
        else if (token_save[index_] == IDENT && operator_num == 0) {
            if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� LIST�� �ƴ�
                    if (isfloat(findNode_horizon_data(save[index_]))) {
                        return_ = stof(findNode_horizon_data(save[index_]));
                        temp += return_;
                    }
                    else {
                        return_ = stoi(findNode_horizon_data(save[index_]));
                        temp += return_;
                    }

                    operator_num++;
                }

            }
        }
        else if (token_save[index_] == IDENT && operator_num == 1) {
            if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� LIST�� �ƴ�
                    if (isfloat(findNode_horizon_data(save[index_]))) {
                        return_ = stof(findNode_horizon_data(save[index_]));
                        temp -= return_;
                    }
                    else {
                        return_ = stoi(findNode_horizon_data(save[index_]));
                        temp -= return_;
                    }
                }

            }
        }
        else if ((token_save[index_] == INT_LIT) && operator_num == 1) { // �ι�° �ǿ����ڸ� ������ ��
            if (isfloat(save[index_])) {
                return_ = stof(save[index_]);
                temp -= return_;
            }
            else {
                temp -= stoi(save[index_]);
            }
        }
    }
    return temp;
}
float div() {
    index_++;
    float temp = 1;
    int operator_num = 0;
    float return_ = 0;
    for (; index_ < save_index; index_++) {
        if (!(save[index_].compare("*")) && operator_num == 1) { // div �Լ� �ȿ��� ���� �����ڸ� ������ �ι�° �ǿ����ڸ� ������ ��
            return_ = mul();
            temp /= return_;
            continue;
        }
        else if (!(save[index_].compare("*")) && operator_num == 0) { // div �Լ� �ȿ��� ���� �����ڸ� ������ ù��° �ǿ����ڸ� ������ ��
            operator_num++;
            return_ = mul();
            temp *= return_;
            continue;
        }
        else if (!(save[index_].compare("+")) && operator_num == 1) { // div �Լ� �ȿ��� ���� �����ڸ� ������ �ι�° �ǿ����ڸ� ������ ��
            return_ = add();
            temp /= return_;
            continue;
        }
        else if (!(save[index_].compare("+")) && operator_num == 0) { // div �Լ� �ȿ��� ���� �����ڸ� ������ ù��° �ǿ����ڸ� ������ ��
            operator_num++;
            return_ = add();
            temp *= return_;
            continue;
        }
        else if (!(save[index_].compare("-")) && operator_num == 1) { // div �Լ� �ȿ��� ���� �����ڸ� ������ �ι�° �ǿ����ڸ� ������ ��
            return_ = sub();
            temp /= return_;
            continue;
        }
        else if (!(save[index_].compare("-")) && operator_num == 0) { // div �Լ� �ȿ��� ���� �����ڸ� ������ ù��° �ǿ����ڸ� ������ ��
            operator_num++;
            return_ = sub();
            temp *= return_;
            continue;
        }
        else if (!(save[index_].compare("/")) && operator_num == 1) { // div �Լ� �ȿ��� ������ �����ڸ� ������ �ι�° �ǿ����ڸ� ������ ��
            return_ = div();
            temp /= return_;
            continue;
        }
        else if (!(save[index_].compare("/")) && operator_num == 0) { // div �Լ� �ȿ��� ������ �����ڸ� ������ ù��° �ǿ����ڸ� ������ ��
            operator_num++;
            return_ = div();
            temp *= return_;
            continue;
        }
        else if (!(save[index_].compare("("))) { // ( ������ ���� �׳� ����ħ
            continue;
        }
        else if (!(save[index_].compare(")"))) { // ) ������ ���� �Լ� ����
            break;
        }
        else if (token_save[index_] == INT_LIT && operator_num == 0) { // ù��° �ǿ����ڸ� ������ ��
            if (isfloat(save[index_])) {
                return_ = stof(save[index_]);
                temp *= return_;
            }
            else {
                temp *= stoi(save[index_]);
            }
            operator_num++;
            continue;
        }
        else if (token_save[index_] == IDENT && operator_num == 0) {
            if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� LIST�� �ƴ�
                    if (isfloat(findNode_horizon_data(save[index_]))) {
                        return_ = stof(findNode_horizon_data(save[index_]));
                        temp *= return_;
                    }
                    else {
                        return_ = stoi(findNode_horizon_data(save[index_]));
                        temp *= return_;
                    }

                    operator_num++;
                }

            }
        }
        else if (token_save[index_] == IDENT && operator_num == 1) {
            if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� LIST�� �ƴ�
                    if (isfloat(findNode_horizon_data(save[index_]))) {
                        return_ = stof(findNode_horizon_data(save[index_]));
                        temp /= return_;
                    }
                    else {
                        return_ = stoi(findNode_horizon_data(save[index_]));
                        temp /= return_;
                    }
                }

            }
        }
        else { // �ι�° �ǿ����ڸ� ������ ��
            if (isfloat(save[index_])) {
                return_ = stof(save[index_]);
                temp /= return_;
            }
            else {
                temp /= stoi(save[index_]);
            }
        }
    }
    return temp;
}

string setq() {
    char stringtemp[1000] = "a";

    string returnstring = "";
    for (; index_ < save_index; index_++) {
        if ((token_save[index_] == IDENT)) {    //�ĺ��ڰ� �� ��

            if (!(save[index_ + 1].compare("NULL"))) {

                append(save[index_], save[index_ + 1], 0); // ���� ����Ʈ�� �߰�
                returnstring = save[index_];
                index_ += 2;
            }
            else if (save[index_ + 1].compare("'") && save[index_ + 1].compare("\"")) { // ������ '�� �ȿ��� ->���� ����, �������̸� index+1�� array ������ ������� ����
                if (save[index_ + 1] == "-") {
                    string temp = "-";
                    temp += save[index_ + 2];
                    append(save[index_], temp, 0);
                    returnstring = save[index_];
                    index_ += 3;
                }
                else { //���
                    append(save[index_], save[index_ + 1], 0);
                    returnstring = save[index_];
                    index_ += 2;
                }
            }
            else if (!(save[index_ + 1].compare("\""))) { // ���ڿ� ����      
                string name = save[index_];

                for (int i = 2; save[index_ + 1] != "\"" || i == 2; i++) {
                    if (i == 2) {
                        strcpy(stringtemp, save[index_ + i].c_str());
                        strcat(stringtemp, " ");
                        index_ += 2;
                    }
                    else {
                        strcat(stringtemp, save[index_ + 1].c_str());
                        strcat(stringtemp, " ");
                        index_++;
                    }
                }
                string data(stringtemp);
                append(name, data, 0);
                returnstring = name;
            }
            else {  //����Ʈ ����

                int indexTemp = index_ + 2; // '�������� ���
                while (1) {  // save�迭 ���� ")"�� �ƴ� ����!  ")"�� ���� ����
                    if (!(save[indexTemp].compare("("))) {
                        indexTemp++;
                    }
                    else if (!(save[indexTemp].compare(")"))) {
                        indexTemp++;
                        break;
                    }
                    else { //���� �� ���.                
                        append(save[index_], save[indexTemp], 1);
                        indexTemp++;

                        returnstring = save[index_];
                    }

                }
                index_ = indexTemp;
            }
        }
        else if (!(save[index_].compare(")")))
            break;
    }

    return returnstring;
}

string* list() {
    string* list_array = new string[100];
    int arrayindex = 0;


    for (; index_ < save_index; index_++) {
        if (token_save[index_] == INT_LIT) { // list�� ������ ������ ���
            list_array[arrayindex] = save[index_]; //�迭�� ���� ����
            arrayindex++;
        }
        else if (!(save[index_].compare(")"))) {
            break;
        }
        else if (!(save[index_].compare("'"))) {
            list_array[arrayindex] = save[index_ + 1];
            arrayindex++;
            index_++; // ����//
        }
        else if (token_save[index_] == IDENT) {
            //��ũ�帮��Ʈ���� �ش� name ã�ƾ� ��.

            if (findNode_horizon_data(save[index_]).compare("NotExist")) { //�����Ұ��.

                if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                    LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                    while (1) {
                        list_array[arrayindex] = checkPoint->data;
                        arrayindex++;

                        if (checkPoint->includeNode == NULL)
                            break;

                        checkPoint = checkPoint->includeNode;
                    }

                }
                else { //�����ϴ� ����� SYMBOL

                    list_array[arrayindex] = findNode_horizon_data(save[index_]);
                    arrayindex++;
                }
            }


        }

    }

    return list_array;
}//��

string car() {
    /*
    1. CAR ( CDR X) ����
    2. CAR X ���� // ���� ������ �Ұ���
    3. CAR '(list data) ���� // ����Ʈ ���� ���� �ִ� ��쿡 '(�� ����ž� ����
    4. �����̸��� �ƴ� ���ڸ� ����Ʈ�� �ְ��� �ϸ� '�� �ٿ��� ��.
    ex) CAR '( 'X) => X

    */

    string* list_array = new string[100];
    int arrayindex = 0;
    int flag = 0;
    for (; index_ < save_index; index_++) {

        if (save[index_].compare("'") == 0 && save[index_ + 1].compare("(") == 0)
            flag = 1;

        if (flag == 1) {  //CAR '(  ) ����. -> ����Ʈ ���� ���� �Է��ϴ� ���.
            if (save[index_].compare("(") == 0) {

                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";

                    while (1) {
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                                list_array[arrayindex] += " ";
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;
                                            list_array[arrayindex] += " ";


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                        list_array[arrayindex] += " ";
                                    }
                                }
                            }

                        }
                        else if (token_save[temp_index] == INT_LIT) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }

                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }
            else if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;

            }
            else if (save[index_].compare(")") == 0) {
                break;
            }
            else if (token_save[index_] == IDENT) {

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }

                    }
                }
            }
        }
        else { // CAR ���� ����Ʈ ������ ����, �ٸ� �Լ��� ���� ���
            if (save[index_].compare("CDR") == 0) { // CAR ( CDR X) ����

                index_++;
                list_array = cdr();   //list_array�� �̹� ���� �����ϴ� ���� ����.
            }
            else if (token_save[index_] == IDENT) {  // CAR X ����

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        //CAR X�����϶�  X�� ����Ʈ�� �ƴ� �ɺ��̸� �Ұ�.

                    }
                }

            }
        }

    }
    return list_array[0];
}

string* cdr() {
    string* list_array = new string[100];
    int arrayindex = 0;
    int flag = 0;
    for (; index_ < save_index; index_++) {

        if (save[index_].compare("'") == 0 && save[index_ + 1].compare("(") == 0)
            flag = 1;

        if (flag == 1) {  //CDR '(  ) ����. -> ����Ʈ ���� ���� �Է��ϴ� ���.
            if (save[index_].compare("(") == 0) {

                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";
                    while (1) {
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                                list_array[arrayindex] += " ";
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;
                                            list_array[arrayindex] += " ";


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                        list_array[arrayindex] += " ";
                                    }
                                }
                            }
                        }
                        else if (token_save[temp_index] == INT_LIT) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }
                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }
            else if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;

            }
            else if (save[index_].compare(")") == 0) {
                break;
            }
            else if (token_save[index_] == IDENT) {

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }

                    }
                }

            }
        }
        else { // CDR ���� ����Ʈ ������ ����, �ٸ� �Լ��� ���� ���
            if (save[index_].compare("CDR") == 0) { // CDR ( CDR X) ����
                index_++;
                list_array = cdr();

                while (!list_array[arrayindex].empty())
                    arrayindex++;

            }
            else if (token_save[index_] == IDENT) {  // CDR X ����

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        /* CDR X <- X�� ����Ʈ ���¸� �����ؾ� �ҵ��ؼ�.
                        else { //�����ϴ� ����� SYMBOL�϶��� ó�� x

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }
                        */

                    }
                }

            }
        }

    }
    string* returnarray = new string[100]; // LIST ù��°���� ���� ����!

    if (arrayindex > 1) {
        for (int i = 1; i < arrayindex; i++) {
            returnarray[i - 1] = list_array[i];
        }

    }

    return returnarray;
} //��

string caddr() {
    string* list_array = new string[100];
    int arrayindex = 0;
    int flag = 0;
    for (; index_ < save_index; index_++) {

        if (save[index_].compare("'") == 0 && save[index_ + 1].compare("(") == 0)
            flag = 1;

        if (flag == 1) {
            if (save[index_].compare("(") == 0) {

                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";

                    while (1) {
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                    }
                                }
                            }

                        }

                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }
            else if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;

            }
            else if (save[index_].compare(")") == 0) {
                break;
            }
            else if (token_save[index_] == IDENT) {

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }

                    }
                }
            }
        }
        else { // CADDR ���� ����Ʈ ������ ����, �ٸ� �Լ��� ���� ���
            if (save[index_].compare("CDR") == 0) { // CADDR ( CDR X) ����

                index_++;
                list_array = cdr();   //list_array�� �̹� ���� �����ϴ� ���� ����.
            }
            else if (token_save[index_] == IDENT) {  // CADDR X ����

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        //CADDR X�����϶�  X�� ����Ʈ�� �ƴ� �ɺ��̸� �Ұ�.

                    }
                }

            }
        }

    }

    if (arrayindex >= 3) {
        return list_array[2];
    }
    else
        return "";
} //��

void cons()
{
    index_++;
    string addData = "";

    string* list_array = new string[100];
    int arrayindex = 0;
    for (; index_ < save_index; index_++) { //cons�� ���� �Ѱ��� �����ϴ� ����
        if (token_save[index_] == IDENT) { //�ĺ����� ��
            if (save[index_ - 1] == "'") {//�տ� '�� ������ �ܼ�����
                addData = save[index_];
                index_++;
                break;
            }
            else {//�׷��������� �̹� ������ִ� ��
                if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                    if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� symbol;
                        addData = findNode_horizon_data(save[index_]);
                        index_++;
                        break;
                    }
                    else {
                        cout << "This is incorrect Data\n" << endl;
                        return;
                    }
                }
            }
        }
        else if (token_save[index_] == INT_LIT) { // �����϶�
            addData = save[index_];
            index_++;
            break;
        }
    }

    list_array[0] = addData;
    arrayindex++;

    int flag = 0;
    for (; index_ < save_index; index_++) {

        if (save[index_].compare("'") == 0 && save[index_ + 1].compare("(") == 0)
            flag = 1;

        if (flag == 1) {  //����Ʈ �� ���� �Է�.
            if (save[index_].compare("(") == 0) {
                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";

                    while (1) { //����Ʈ ��ø
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                                list_array[arrayindex] += " ";
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;
                                            list_array[arrayindex] += " ";


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                        list_array[arrayindex] += " ";
                                    }
                                }
                            }

                        }
                        else if (token_save[temp_index] == INT_LIT) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }

                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }
            else if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;
            }
            else if (save[index_].compare(")") == 0) {
                break;
            }
            else if (token_save[index_] == IDENT) {

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }

                    }
                }
            }
        }
        else {

            if (token_save[index_] == IDENT) {
                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }


                    }
                }

            }
        }

    }

    if (arrayindex >= 1) {
        cout << "(";
        for (int i = 0; i < arrayindex; i++) {

            if (list_array[i].length() > 1 && list_array[i][0] == '(') { //����Ʈ ��ø�� ���

                stringstream ss(list_array[i]);
                string k;
                if (ss >> k)
                    cout << k;

                if (ss >> k)
                    cout << k;
                while (ss >> k) {
                    if (k == ")") {
                        if (i != arrayindex - 1)
                            cout << k << " ";
                        else
                            cout << k;
                    }
                    else
                        cout << " " << k;
                }

            }
            else if (i == arrayindex - 1) {
                cout << list_array[i];
            }
            else {
                cout << list_array[i] << " ";
            }

        }
        cout << ")" << endl;
    }


}

void reverse()
{
    index_++;
    int flag = 0;
    int arrayindex = 0;
    string* list_array = new string[100];
    for (; index_ < save_index; index_++) {  //����Ʈ �������

        if (save[index_].compare("'") == 0 && save[index_ + 1].compare("(") == 0)
            flag = 1;

        if (flag == 1) {  //����Ʈ �� ���� �Է�.
            if (save[index_].compare("(") == 0) {
                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";

                    while (1) { //����Ʈ ��ø
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                                list_array[arrayindex] += " ";
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;
                                            list_array[arrayindex] += " ";


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                        list_array[arrayindex] += " ";
                                    }
                                }
                            }

                        }
                        else if (token_save[temp_index] == INT_LIT) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }

                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }
            else if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;
            }
            else if (save[index_].compare(")") == 0) {
                break;
            }
            else if (token_save[index_] == IDENT) {

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }

                    }
                }
            }
        }
        else {

            if (token_save[index_] == IDENT) {
                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }


                    }
                }

            }
        }

    }

    if (arrayindex >= 1) { //����Ʈ ��°��� 
        cout << "(";
        for (int i = arrayindex - 1; i >= 0; i--) {

            if (list_array[i].length() > 1 && list_array[i][0] == '(') {

                stringstream ss(list_array[i]);
                string k;
                if (ss >> k)
                    cout << k;

                if (ss >> k)
                    cout << k;
                while (ss >> k) {
                    if (k == ")") {
                        if (i != arrayindex - 1)
                            cout << k << " ";
                        else
                            cout << k;
                    }
                    else
                        cout << " " << k;
                }

            }
            else if (i == 0) {
                cout << list_array[i];
            }
            else {
                cout << list_array[i] << " ";
            }

        }
        cout << ")" << endl;
    }
}

void appendlist()
{
    LISP_list* find;
    int countcomma = 0;
    int i = 0;
    string temp[100];

    for (; index_ < save_index; index_++)
    {
        if (save[index_] == "'" && save[index_ + 1] == "(")
            countcomma++;

        if (save[index_] == ")")
            countcomma--;

        if (token_save[index_] == IDENT && countcomma == 0)
        {
            if (head == NULL)
            {
                cout << "Append�ϰ��� �ϴ� ����Ʈ�� �������� �ʽ��ϴ�." << endl;
                break;
            }

            find = head;
            while (find->name != save[index_])
            {
                find = find->nextNode;
                if (find == NULL)
                {
                    cout << "Append�ϰ��� �ϴ� ����Ʈ�� �������� �ʽ��ϴ�." << endl;
                    break;
                }
            }

            while (find != NULL)
            {
                temp[i] = find->data;
                i++;
                find = find->includeNode;
            }
        }

        if ((token_save[index_] == IDENT || token_save[index_] == INT_LIT) && countcomma == 1)
        {
            temp[i] = save[index_];
            i++;
        }
    }

    if (i == 0)
        return;

    cout << "(";
    for (int j = 0; j < i; j++)
    {
        if (j == i - 1)
            cout << temp[j];
        else
            cout << temp[j] << " ";
    }
    cout << ")" << endl;
}

void length()
{
    int count = 0;
    int bracket = 0;

    for (; index_ < save_index; index_++)
    {
        if (token_save[index_] == LEFT_PAREN) //���� ��ȣ +ī��Ʈ
            bracket++;
        if ((token_save[index_] == IDENT || token_save[index_] == INT_LIT) && bracket == 1) //lengthī��Ʈ
            count++;
        if (token_save[index_] == RIGHT_PAREN) //�����ʰ�ȣ�� ����
        {
            bracket--;
            if (bracket == 1)
                count++;
        }
    }
    cout << count << endl;
}

bool atom() {
    index_++;
    LISP_list* temp = head;
    if (isINTorFLOAT(save[index_])) {
        return true;
    }

    if (head == NULL) { // � symbol�̳� list�� ����Ǿ����� ���� ���
      //  printf("NIL");
        return false;
    }
    else { // �׷��� ���� ���
        do {
            if (!(save[index_].compare(temp->name))) { // �̸��� ���ٸ�
                if (temp->includeNode == NULL) { // list�� �ƴ϶��
                   // printf("T\n");
                    return true;
                }
            }
            temp = temp->nextNode;
        } while (temp != NULL); // ���̻� ����� ���� �ִ��� Ȯ��
    }
    return false;
}

bool null() {
    index_++;
    LISP_list* temp = head;
    if (head == NULL) { // � symbol�̳� list�� ����Ǿ����� ���� ���
        return true;
    }
    else { // �׷��� ���� ���
        while (temp != NULL) {
            if (!(save[index_].compare(temp->name))) { // �̸��� ���ٸ�
                if ((temp->data) == "NULL")
                    return true;
                else
                    return false;

            }
            temp = temp->nextNode;
        }
    }
    return true;
}

bool numberp() { // ���ڰ� �� ���� ����ǵ��� ���ľ� ��
    index_++;
    LISP_list* temp = head;

    if (isINTorFLOAT(save[index_])) {
        return true;
    }
    else {
        if (head == NULL) { // � symbol�̳� list�� ����Ǿ����� ���� ���
            return false;
        }
        else { // �׷��� ���� ���
            while (temp != NULL) {
                if (!(save[index_].compare(temp->name))) { // �̸��� ���ٸ�
                    if (temp->includeNode == NULL && isINTorFLOAT(temp->data)) { // list�� �ƴϰ� symbol�� ���� ���ڶ��               
                        return true;
                    }
                }
                temp = temp->nextNode;
            } // ���̻� ����� ���� �ִ��� Ȯ��
        }
    }
    return false;
}

bool isINTorFLOAT(string a) { //����, �Ǽ��� ���� true�� ��ȯ
    const char* temp = a.c_str();
    bool value = false;

    int dotCount = 0;
    for (int i = 0; temp[i] != '\0'; i++) {

        if (i == 0) { //ù���ڴ� - Ȥ�� ���ڰ� �ü�����
            if ((temp[i] == '-') || ('0' <= temp[i] && temp[i] <= '9'))
                value = true;
            else {
                value = false;
                break;
            }
        }
        else { //���������� ���� Ȥ�� �Ҽ����� �ü�����, �Ҽ����� 1�� ���ϸ� �ü�����
            if (('0' <= temp[i] && temp[i] <= '9')) {
                value = true;
            }
            else if ((temp[i] == '.')) {
                dotCount++;
                if (dotCount > 1) {
                    value = false;
                    break;
                }
                else
                    value = true;
            }
            else {
                value = false;
                break;
            }
        }

    }
    return value;
}

bool zerop() {
    index_++;
    LISP_list* temp = head;

    if (isINTorFLOAT(save[index_])) {
        if (!(save[index_].compare("0"))) {
            index_ += 1;
            return true;
        }
    }

    if (head == NULL) { // � symbol�̳� list�� ����Ǿ����� ���� ��� 
        return false;
    }
    else { // �׷��� ���� ���
        while (temp != NULL) {
            if (!(save[index_].compare(temp->name))) { // �̸��� ���ٸ�
                if (temp->includeNode == NULL) { // list�� �ƴϰ� symbol�� ���� ���ڶ��
                    if (isINTorFLOAT(temp->data)) {
                        if (stoi(temp->data) == 0) { //data�� 0�϶� (string to integer)
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                    else { //���ڰ� �ƴ� ��
                        cout << "ERROR" << endl;
                        return false;
                    }
                }
            }
            temp = temp->nextNode;
        }
    }
    cout << "ERROR" << endl;
    return false;

}

bool minusp() { // ���ڰ� �� ���� ����ǵ��� ���ľ� �� -> ��ħ
    index_++;
    LISP_list* temp = head;

    if (isINTorFLOAT(save[index_])) {
        if (atoi(save[index_].c_str()) < 0) {
            return true;
        }
        else {
            return false;
        }
    }

    if (head == NULL) { // � symbol�̳� list�� ����Ǿ����� ���� ���

        return false;
    }
    else { // �׷��� ���� ���
        while (temp != NULL) {
            if (!(save[index_].compare(temp->name))) { // �̸��� ���ٸ�
                if (temp->includeNode == NULL) { // list�� �ƴϰ� symbol�� ���� ���ڶ��
                    if (isINTorFLOAT(temp->data) == true) {
                        if (atoi((temp->data).c_str()) < 0) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                    else {
                        printf("ERROR\n");
                        return false;
                    }
                }
            }
            temp = temp->nextNode;
        } // ���̻� ����� ���� �ִ��� Ȯ��
    }
    printf("ERROR\n");
    return false;
}

bool equal() {
    index_++;
    string data1, data2;
    int flag1 = 0, flag2 = 0;
    LISP_list* temp1 = head;
    LISP_list* temp2 = head;

    while (temp1 != NULL) {
        if (!(save[index_].compare(temp1->name))) {
            if (temp1->includeNode == NULL) { //����Ʈ�� �ƴϸ�
                data1 = temp1->data;
                flag1 = 1;
                break;
            }
        }
        temp1 = temp1->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data1 = save[index_];
        flag1 = 1;
    }

    if (flag1 == 0) {
        cout << "ERROR" << endl;
        return false;
    }

    index_++;
    while (temp2 != NULL) {
        if (!(save[index_].compare(temp2->name))) {
            if (temp2->includeNode == NULL) {
                data2 = temp2->data;
                flag2 = 1;
                break;
            }
        }
        temp2 = temp2->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data2 = save[index_];
        flag2 = 1;
    }

    if (flag2 == 0) {
        cout << "ERROR" << endl;

        return false;
    }

    if (atoi(data1.c_str()) == atoi(data2.c_str())) { return true; }
    else { return false; }

}

bool gt() {
    index_++;
    string data1, data2;
    int flag1 = 0, flag2 = 0;
    LISP_list* temp1 = head;
    LISP_list* temp2 = head;

    while (temp1 != NULL) {
        if (!(save[index_].compare(temp1->name))) {
            if (temp1->includeNode == NULL) {
                data1 = temp1->data;
                flag1 = 1;
                break;
            }
        }
        temp1 = temp1->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data1 = save[index_];
        flag1 = 1;
    }

    if (flag1 == 0) {
        printf("ERROR\n");
        return false;
    }

    index_++;
    while (temp2 != NULL) {

        if (!(save[index_].compare(temp2->name))) {
            if (temp2->includeNode == NULL) {
                data2 = temp2->data;
                flag2 = 1;
                break;
            }
        }
        temp2 = temp2->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data2 = save[index_];
        flag2 = 1;
    }

    if (flag2 == 0) {
        printf("ERROR\n");
        return false;
    }

    if (atoi(data1.c_str()) > atoi(data2.c_str())) {
        return true;
    }
    else {
        return false;
    }
}

bool ge() {
    index_ += 2;
    string data1, data2;
    int flag1 = 0, flag2 = 0;
    LISP_list* temp1 = head;
    LISP_list* temp2 = head;

    while (temp1 != NULL) {
        if (!(save[index_].compare(temp1->name))) {
            if (temp1->includeNode == NULL) {
                data1 = temp1->data;
                flag1 = 1;
                break;
            }
        }
        temp1 = temp1->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data1 = save[index_];
        flag1 = 1;
    }

    if (flag1 == 0) {
        printf("ERROR\n");
        return false;
    }

    index_++;

    while (temp2 != NULL) {
        if (!(save[index_].compare(temp2->name))) {
            if (temp2->includeNode == NULL) {
                data2 = temp2->data;
                flag2 = 1;
                break;
            }
        }
        temp2 = temp2->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data2 = save[index_];
        flag2 = 1;
    }

    if (flag2 == 0) {
        printf("ERROR\n");
        return false;
    }

    if (atoi(data1.c_str()) >= atoi(data2.c_str())) {
        return true;
    }
    else {
        return false;
    }
}

bool lt() {
    index_++;
    string data1, data2;
    int flag1 = 0, flag2 = 0;
    LISP_list* temp1 = head;
    LISP_list* temp2 = head;

    while (temp1 != NULL) {
        if (!(save[index_].compare(temp1->name))) {
            if (temp1->includeNode == NULL) {
                data1 = temp1->data;
                flag1 = 1;
                break;
            }
        }
        temp1 = temp1->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data1 = save[index_];
        flag1 = 1;
    }

    if (flag1 == 0) {
        printf("ERROR\n");
        return false;
    }

    index_++;
    while (temp2 != NULL) {
        if (!(save[index_].compare(temp2->name))) {
            if (temp2->includeNode == NULL) {
                data2 = temp2->data;
                flag2 = 1;
                break;
            }
        }
        temp2 = temp2->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data2 = save[index_];
        flag2 = 1;
    }

    if (flag2 == 0) {
        printf("ERROR\n");
        return false;
    }

    if (atoi(data1.c_str()) < atoi(data2.c_str())) {
        return true;
    }
    else {
        return false;
    }
}

bool le() {
    index_ += 2;
    string data1, data2;
    int flag1 = 0, flag2 = 0;
    LISP_list* temp1 = head;
    LISP_list* temp2 = head;

    while (temp1 != NULL) {
        if (!(save[index_].compare(temp1->name))) {
            if (temp1->includeNode == NULL) {
                data1 = temp1->data;
                flag1 = 1;
                break;
            }
        }
        temp1 = temp1->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data1 = save[index_];
        flag1 = 1;
    }

    if (flag1 == 0) {
        printf("ERROR\n");
        return false;
    }

    index_++;
    while (temp2 != NULL) {
        if (!(save[index_].compare(temp2->name))) {
            if (temp2->includeNode == NULL) {
                data2 = temp2->data;
                flag2 = 1;
                break;
            }
        }
        temp2 = temp2->nextNode;
    }

    // save[index_]�� ���ڶ�� data1=save[index_];
    if (isINTorFLOAT(save[index_])) {
        data2 = save[index_];
        flag2 = 1;
    }

    if (flag2 == 0) {
        printf("ERROR\n");
        return false;
    }

    if (atoi(data1.c_str()) <= atoi(data2.c_str())) {
        return true;
    }
    else { //nil
        return false;
    }
}


string nth() {
    index_++;
    int flag = 0;
    int nth_record = -1;
    string* list_array = new string[100];
    int arrayindex = 0;
    for (; index_ < save_index; index_++) {
        if (token_save[index_] == INT_LIT) {
            nth_record = stoi(save[index_]);
            break;
        }
        else {
            return "NIL";
        }
    }

    for (; index_ < save_index; index_++) {
        if (save[index_].compare("'") == 0 && save[index_ + 1].compare("(") == 0)
            flag = 1;


        if (flag) {
            cout << "dada";
            if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;
            }
            else if (token_save[index_] == IDENT) {
                if ((save[index_ - 1].compare("'")) == 0) { // ���ڱ���ü
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else { //�����ΰ��
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL
                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }
                    }

                }
            }

        }
        else {
            if (token_save[index_] == IDENT) {

                if (save[index_ - 1] == "'") {
                    return "ERROR";
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else {
                            return "ERROR";
                        }

                    }
                }
            }
        }


    }

    if (nth_record == -1)
        return "ERROR";


    if (arrayindex < nth_record) {
        if (flag == 0)
            return "ERROR";
        else
            return "NIL";
    }
    else {
        return list_array[nth_record];
    }


}

bool stringp() {
    index_++;
    if (isINTorFLOAT(save[index_])) { //�����̸� false
        return false;
    }
    else if (token_save[index_] == IDENT) {
        LISP_list* temp = head;
        while (temp != NULL) {
            if (!(save[index_].compare(temp->name))) {
                if (temp->includeNode == NULL) { //����Ʈ�� �ƴϸ鼭
                    if (isINTorFLOAT(temp->data)) return false;  //���ڸ� false
                    else return true; //�ƴϸ� true

                }
            }
            temp = temp->nextNode;
        }
    }
    else {
        if (!(save[index_].compare("'"))) {
            return false;
        }
        else if (!(save[index_].compare("#"))) {
            return false;
        }
        else if (!(save[index_].compare("\""))) {
            return true;
        }
        else if (!(save[index_].compare("NIL"))) {
            return false;
        }
    }
}

string* member() {
    index_++;
    string searchData = "";
    string* temp = new string[100];
    string* list_array = new string[100];
    for (; index_ < save_index; index_++) {
        if (token_save[index_] == IDENT) {
            if (save[index_ - 1] == "'") {
                searchData = save[index_];
                index_++;
                break;
            }
            else {
                cout << "ã���� �ϴ� ���� ������ �ùٸ� ���·� �Է��ϼ���. (' + ����)" << endl;
                temp[0] = "NIL";
                return temp;
            }
        }
        else if (token_save[index_] == INT_LIT) {
            searchData = save[index_];
            index_++;
            break;
        }
    }



    int arrayindex = 0;
    int flag = 0;

    for (; index_ < save_index; index_++) {

        if (save[index_] == "'" && save[index_ + 1] == "(") {
            flag = 1;
        }

        if (flag == 1) { //����Ʈ ���� �Է�
            if (save[index_].compare("(") == 0) {

                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";

                    while (1) {
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                    }
                                }
                            }

                        }
                        else if (token_save[temp_index] == INT_LIT) {
                            list_array[arrayindex] += save[temp_index];
                        }

                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }
            else if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;

            }
            else if (save[index_].compare(")") == 0) {
                break;
            }
            else if (token_save[index_] == IDENT) {

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }

                    }
                }
            }
        }
        else { // ����Ʈ �ҷ�����
            if (token_save[index_] == IDENT) {  // MEMBER afa X ����

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }


                    }
                }

            }
        }


    }

    int storemode = 0;
    int tempindex = 0;

    for (int i = 0; i < arrayindex; i++) {
        if (storemode == 1) {
            temp[tempindex] = list_array[i];
            tempindex++;
        }
        else if (list_array[i] == searchData && storemode == 0) {
            storemode = 1;
            temp[tempindex] = list_array[i];
            tempindex++;
        }
    }

    if (storemode == 0) {
        temp[0] = "NIL";
        //return temp;
    }

    return temp;
}

string assoc() {
    index_++;
    string searchData = "";
    string temp = "";
    string* list_array = new string[100];
    for (; index_ < save_index; index_++) {
        if (token_save[index_] == IDENT) {
            if (save[index_ - 1] == "'") {
                searchData = save[index_];
                index_++;
                break;
            }
            else {
                cout << "ã���� �ϴ� ���� ������ �ùٸ� ���·� �Է��ϼ���. (' + ����)" << endl;
                temp = "NIL";
                return temp;
            }
        }
        else if (token_save[index_] == INT_LIT) {
            searchData = save[index_];
            index_++;
            break;
        }
    }
    int arrayindex = 0;
    int flag = 0;

    for (; index_ < save_index; index_++) {

        if (save[index_] == "'" && save[index_ + 1] == "(") {
            flag = 1;
        }

        if (flag == 1) { //����Ʈ ���� �Է�
            if (save[index_].compare("(") == 0) {

                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";

                    while (1) {
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                                list_array[arrayindex] += " ";
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;
                                            list_array[arrayindex] += " ";


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                        list_array[arrayindex] += " ";
                                    }
                                }
                            }

                        }
                        else if (token_save[temp_index] == INT_LIT) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }

                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }

        }
        else { // ����Ʈ �ҷ�����
            if (token_save[index_] == IDENT) {  // MEMBER afa X ����

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        //CAR X�����϶�  X�� ����Ʈ�� �ƴ� �ɺ��̸� �Ұ�.

                    }
                }

            }
        }


    }

    stringstream ss;
    int flag_ = 0;
    int index = 0;
    for (index = 0; index < arrayindex; index++) {

        string temp = "";
        ss.str(list_array[index]);
        ss >> temp;
        ss >> temp;


        if (temp == searchData) {
            flag_ = 1;
            break;
        }

        ss.clear();
    }

    if (flag_ == 1) {
        return  list_array[index];
    }
    else {
        return "NIL";
    }
}

void remove() {
    index_++;
    string* list_array = new string[100];
    string searchData = "";
    int arrayindex = 0;
    int flag = 0;

    for (; index_ < save_index; index_++) {
        if (token_save[index_] == IDENT) {
            if (save[index_ - 1] == "'") {
                searchData = save[index_];
                index_++;
                break;
            }
            else {
                if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                    if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� symbol;
                        searchData = findNode_horizon_data(save[index_]);
                        index_++;
                        break;
                    }
                    else
                        cout << "This is List Data\n" << endl;

                }


            }
        }
        else if (token_save[index_] == INT_LIT) {
            searchData = save[index_];
            index_++;
            break;
        }
    }


    for (; index_ < save_index; index_++) {

        if (save[index_].compare("'") == 0 && save[index_ + 1].compare("(") == 0)
            flag = 1;

        if (flag == 1) {  // -> ����Ʈ ���� ���� �Է��ϴ� ���.
            if (save[index_].compare("(") == 0) {
                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";

                    while (1) {
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                                list_array[arrayindex] += " ";
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;
                                            list_array[arrayindex] += " ";


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                        list_array[arrayindex] += " ";
                                    }
                                }
                            }

                        }
                        else if (token_save[temp_index] == INT_LIT) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }

                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }
            else if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;

            }
            else if (save[index_].compare(")") == 0) {
                break;
            }
            else if (token_save[index_] == IDENT) {

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }

                    }
                }
            }
        }
        else { //
            if (token_save[index_] == IDENT) {  // CAR X ����

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }


                    }
                }

            }
        }
    }

    cout << "(";

    for (int i = 0; i < arrayindex; i++) {
        if (list_array[i] != searchData) {

            if (i == arrayindex - 1)
                cout << list_array[i];
            else
                cout << list_array[i] << " ";
        }
    }
    cout << ")" << endl;

}
void subst() {
    index_++;
    string* list_array = new string[100];
    string FirstData = "";
    string SecondData = "";
    int arrayindex = 0;
    int flag = 0;

    for (; index_ < save_index; index_++) {
        if (token_save[index_] == IDENT) {
            if (save[index_ - 1] == "'") {
                FirstData = save[index_];
                index_++;
                break;
            }
            else {
                if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                    if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� symbp;
                        FirstData = findNode_horizon_data(save[index_]);
                        index_++;
                        break;
                    }
                    else
                        cout << "This is List Data\n" << endl;

                }


            }
        }
        else if (token_save[index_] == INT_LIT) {
            FirstData = save[index_];
            index_++;
            break;
        }
    }

    for (; index_ < save_index; index_++) {
        if (token_save[index_] == IDENT) {
            if (save[index_ - 1] == "'") {
                SecondData = save[index_];
                index_++;
                break;
            }
            else {
                if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                    if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) != 0) { // �����ϴ� ����� symbp;
                        SecondData = findNode_horizon_data(save[index_]);
                        index_++;
                        break;
                    }
                    else
                        cout << "This is List Data\n" << endl;

                }


            }
        }
        else if (token_save[index_] == INT_LIT) {
            SecondData = save[index_];
            index_++;
            break;
        }
    }


    for (; index_ < save_index; index_++) {

        if (save[index_].compare("'") == 0 && save[index_ + 1].compare("(") == 0)
            flag = 1;

        if (flag == 1) {  //CAR '(  ) ����. -> ����Ʈ ���� ���� �Է��ϴ� ���.
            if (save[index_].compare("(") == 0) {
                if (save[index_ - 1].compare("'") != 0) {
                    int temp_index = index_;
                    list_array[arrayindex] = "";

                    while (1) {
                        if (save[temp_index].compare("(") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }
                        else if (save[temp_index].compare(")") == 0) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                            break;
                        }
                        else if (token_save[temp_index] == IDENT) {

                            if ((save[temp_index - 1].compare("'")) == 0) { //���� �״��.
                                list_array[arrayindex] += save[temp_index];
                                list_array[arrayindex] += " ";
                            }
                            else { // ���ڿ� �ش��ϴ� �̸��� ���� ���� �� ���� �����;� ��.


                                if (findNode_horizon_data(save[temp_index]).compare("NotExist")) {  // symbol or list�� �����ϸ�

                                    if ((findNode_horizon_data(save[temp_index]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                                        LISP_list* checkPoint = findNode_horizon_addr(save[temp_index]);

                                        while (1) {
                                            list_array[arrayindex] += checkPoint->data;
                                            list_array[arrayindex] += " ";


                                            if (checkPoint->includeNode == NULL)
                                                break;

                                            checkPoint = checkPoint->includeNode;
                                        }

                                    }
                                    else { //�����ϴ� ����� SYMBOL
                                        list_array[arrayindex] += findNode_horizon_data(save[index_]);
                                        list_array[arrayindex] += " ";
                                    }
                                }
                            }

                        }
                        else if (token_save[temp_index] == INT_LIT) {
                            list_array[arrayindex] += save[temp_index];
                            list_array[arrayindex] += " ";
                        }

                        temp_index++;
                    }

                    arrayindex++;
                    index_ = temp_index;
                }
            }
            else if (token_save[index_] == INT_LIT) {
                list_array[arrayindex] = save[index_];
                arrayindex++;

            }
            else if (save[index_].compare(")") == 0) {
                break;
            }
            else if (token_save[index_] == IDENT) {

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {

                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        else { //�����ϴ� ����� SYMBOL

                            list_array[arrayindex] = findNode_horizon_data(save[index_]);
                            arrayindex++;
                        }

                    }
                }
            }
        }
        else { // CAR ���� ����Ʈ ������ ����, �ٸ� �Լ��� ���� ���
            if (token_save[index_] == IDENT) {  // CAR X ����

                if ((save[index_ - 1].compare("'")) == 0) { //���� �״��.
                    list_array[arrayindex] = save[index_];
                    arrayindex++;
                }
                else {
                    if (findNode_horizon_data(save[index_]).compare("NotExist")) {

                        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                            while (1) {
                                list_array[arrayindex] = checkPoint->data;
                                arrayindex++;

                                if (checkPoint->includeNode == NULL)
                                    break;

                                checkPoint = checkPoint->includeNode;
                            }

                        }
                        //CAR X�����϶�  X�� ����Ʈ�� �ƴ� �ɺ��̸� �Ұ�.

                    }
                }

            }
        }
    }

    for (int i = 0; i < arrayindex; i++) {
        if (list_array[i] == SecondData)
            list_array[i] = FirstData;
    }

    cout << "(";
    //����Ʈ�� ����Ʈ ó�� �Ұ�����
    for (int i = 0; i < arrayindex; i++) {

        if (i == arrayindex - 1)
            cout << list_array[i];
        else
            cout << list_array[i] << " ";

    }
    cout << ")" << endl;

}

bool iif() {
    index_++;
    int left_flag = 0;
    int right_flag = 0;
    for (; index_ < save_index; index_++) {
        if (!(save[index_].compare(">"))) {
            if (!(save[index_ + 1].compare("="))) {
                if (ge()) {
                    //���ǹ��� ���̶��
                    index_ += 2;
                    bool tempm = tempmain();
                    if (tempm == true) return true;
                }
                else {
                    // ���ǹ��� �����̶��
                    index_ += 2;
                    while (true) {
                        if (!(save[index_].compare("("))) left_flag++;
                        if (!(save[index_].compare(")"))) right_flag++;
                        index_++;
                        if (left_flag == 1 && right_flag == 1 && !(save[index_].compare(")"))) {
                            index_ += 2;
                            return false;
                        }
                        else if (left_flag == 1 && right_flag == 1 && !(save[index_].compare("("))) {
                            tempmain();
                            return false;
                        }
                    }
                }
            }
            else {
                if (gt()) {
                    //���ǹ��� ���̶��
                    index_ += 2;
                    bool tempm = tempmain();
                    if (tempm == true) return true;
                }
                else {
                    // ���ǹ��� �����̶��
                    index_ += 2;
                    while (true) {
                        if (!(save[index_].compare("("))) left_flag++;
                        if (!(save[index_].compare(")"))) right_flag++;
                        index_++;
                        if (left_flag == 1 && right_flag == 1 && !(save[index_].compare(")"))) {
                            index_ += 2;
                            return false;
                        }
                        else if (left_flag == 1 && right_flag == 1 && !(save[index_].compare("("))) {
                            tempmain();
                            return false;
                        }
                    }
                }
            }
        }
        else if (!(save[index_].compare("<"))) {
            if (!(save[index_].compare("="))) {
                if (le()) {
                    //���ǹ��� ���̶��
                    index_ += 2;
                    bool tempm = tempmain();
                    if (tempm == true) return true;
                }
                else {
                    // ���ǹ��� �����̶��
                    index_ += 2;
                    while (true) {
                        if (!(save[index_].compare("("))) left_flag++;
                        if (!(save[index_].compare(")"))) right_flag++;
                        index_++;
                        if (left_flag == 1 && right_flag == 1 && !(save[index_].compare(")"))) {
                            index_ += 2;
                            return false;
                        }
                        else if (left_flag == 1 && right_flag == 1 && !(save[index_].compare("("))) {
                            tempmain();
                            return false;
                        }
                    }
                }
            }
            else {
                if (lt()) {
                    //���ǹ��� ���̶��
                    index_ += 2;
                    bool tempm = tempmain();
                    if (tempm == true) return true;
                }
                else {
                    // ���ǹ��� �����̶��
                    index_ += 2;
                    while (true) {
                        if (!(save[index_].compare("("))) left_flag++;
                        if (!(save[index_].compare(")"))) right_flag++;
                        index_++;
                        if (left_flag == 1 && right_flag == 1 && !(save[index_].compare(")"))) {
                            index_ += 2;
                            return false;
                        }
                        else if (left_flag == 1 && right_flag == 1 && !(save[index_].compare("("))) {
                            tempmain();
                            return false;
                        }
                    }
                }
            }
        }
        else if (!(save[index_].compare("EQUAL")) || save[index_] == "=") {
            if (equal()) {
                //���ǹ��� ���̶��
                index_ += 2;
                bool tempm = tempmain();
                if (tempm == true) return true;
            }
            else {
                // ���ǹ��� �����̶��
                index_ += 2;
                while (true) {
                    if (!(save[index_].compare("("))) left_flag++;
                    if (!(save[index_].compare(")"))) right_flag++;
                    index_++;
                    if (left_flag == 1 && right_flag == 1 && !(save[index_].compare(")"))) {
                        index_ += 2;
                        return false;
                    }
                    else if (left_flag == 1 && right_flag == 1 && !(save[index_].compare("("))) {
                        tempmain();
                        return false;
                    }
                }
            }
        }
        else if (!(save[index_].compare("("))) { continue; }
    }
}

bool cond() {
    index_ += 2;
    for (; save[index_].compare(";") != 0;) {
        if (iif() == true) {
            return true;
            break;
        }
    }
}

bool tempmain() {
    index_++;
    if (!(save[index_].compare("+"))) // ���� ����
    {
        cout << add() << endl;
        return true;
    }
    else if (!(save[index_].compare("*"))) // ���� ����
    {
        cout << mul() << endl;
        return true;
    }
    else if (!(save[index_].compare("-"))) // ���� ����
    {
        cout << sub() << endl;
        return true;
    }
    else if (!(save[index_].compare("/"))) // ������ ����
    {
        cout << div() << endl;
        return true;
    }
    else if (!(save[index_].compare("SETQ")) || !(save[index_].compare("setq"))) {
        string temp = setq();
        string printdata;
        if ((findNode_horizon_data(temp).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
            LISP_list* checkPoint = findNode_horizon_addr(temp);

            while (1) {
                printdata += checkPoint->data;
                printdata += " ";

                if (checkPoint->includeNode == NULL)
                    break;

                checkPoint = checkPoint->includeNode;
            }
            stringstream ss(printdata);
            string k;
            cout << "(";
            ss >> k;
            cout << k;
            while (ss >> k) {
                cout << " " << k;

            }
            cout << ")" << endl;

        }
        else {
            printdata = findNode_horizon_data(temp);
            cout << printdata << endl;
        }
        return true;
    }
    else if (!(save[index_].compare("LIST")) || !(save[index_].compare("list"))) {
        string* array;
        array = list();
        int arrayindex = 0;
        while (!array[arrayindex].empty())
            arrayindex++;

        if (arrayindex >= 1) {

            cout << "(";
            for (int i = 0; i < arrayindex; i++) {


                if (i == arrayindex - 1)
                    cout << array[i];
                else
                    cout << array[i] << " ";

            }
            cout << ")" << endl;

        }
        return true;
    }
    else if (!(save[index_].compare("CAR")) || !(save[index_].compare("car"))) {
        string str;
        index_++;
        str = car();

        if (str.length() > 1 && str[0] == '(') {
            stringstream ss(str);
            string k;
            if (ss >> k)
                cout << k;
            if (ss >> k)
                cout << k;
            while (ss >> k) {
                if (k == ")")
                    cout << k << endl;
                else
                    cout << " " << k;
            }
        }
        else {
            cout << str << endl;
        }
        return true;
    }
    else if (!(save[index_].compare("CDR")) || !(save[index_].compare("cdr"))) {
        index_++;
        string* list_array = cdr();

        int arrayindex = 0;
        while (!list_array[arrayindex].empty())
            arrayindex++;

        if (arrayindex >= 1) {
            cout << "(";
            for (int i = 0; i < arrayindex; i++) {
                if (list_array[i].length() > 1 && list_array[i][0] == '(') {
                    stringstream ss(list_array[i]);
                    string k;
                    if (ss >> k)
                        cout << k;
                    if (ss >> k)
                        cout << k;
                    while (ss >> k) {
                        if (k == ")") {
                            if (i != arrayindex - 1)
                                cout << k << " ";
                            else
                                cout << k;
                        }
                        else
                            cout << " " << k;
                    }
                }
                else if (i == arrayindex - 1) {
                    cout << list_array[i];
                }
                else {
                    cout << list_array[i] << " ";
                }
            }
            cout << ")" << endl;
        }
        return true;
    }
    else if (!(save[index_].compare("CADDR")) || !(save[index_].compare("caddr"))) {
        string str;
        index_++;
        str = caddr();

        if (str.length() > 1 && str[0] == '(') { // list����
            for (int i = 0; i < str.length(); i++) {
                if ((i == 0) || (i == str.length() - 2) || (i == str.length() - 1))
                    cout << str.substr(i, 1);
                else
                    cout << str.substr(i, 1) << " ";
            }
            cout << endl;
        }
        else {
            cout << str << endl;
        }
        return true;
    }
    else if (!(save[index_].compare("NTH")) || !(save[index_].compare("nth"))) {
        string temp = nth();
        cout << temp << endl;
        return true;
    }
    else if (!(save[index_].compare("CONS")) || !(save[index_].compare("cons"))) {
        cons();
        return true;
    }
    else if (!(save[index_].compare("REVERSE")) || !(save[index_].compare("reverse"))) {
        reverse();
        return true;
    }
    else if (!(save[index_].compare("APPEND")) || !(save[index_].compare("append"))) {
        appendlist();
        return true;
    }
    else if (!(save[index_].compare("LENGTH")) || !(save[index_].compare("length"))) {
        length();
        return true;
    }
    else if (!(save[index_].compare("MEMBER")) || !(save[index_].compare("member"))) {
        string* list_array = member();
        int arrayindex = 0;
        while (!list_array[arrayindex].empty())
            arrayindex++;

        if (arrayindex >= 1) {
            cout << "(";
            for (int i = 0; i < arrayindex; i++) {
                if (list_array[i].length() > 1 && list_array[i][0] == '(') {
                    for (int j = 0; j < list_array[i].length(); j++) {
                        if ((j == 0) || (j == list_array[i].length() - 2))
                            cout << list_array[i].substr(j, 1);
                        else if ((j == list_array[i].length() - 1)) {
                            if (i == arrayindex - 1)
                                cout << list_array[i].substr(j, 1);
                            else
                                cout << list_array[i].substr(j, 1) << " ";
                        }
                        else
                            cout << list_array[i].substr(j, 1) << " ";
                    }
                }
                else if (i == arrayindex - 1) {
                    cout << list_array[i];
                }
                else {
                    cout << list_array[i] << " ";
                }
            }
            cout << ")" << endl;
        }
        index_ = save_index - 1;
        return true;
    }
    else if (!(save[index_].compare("ASSOC")) || !(save[index_].compare("assoc"))) {
        string temp = assoc();
        stringstream ss(temp);
        string k;
        if (ss >> k)
            cout << k;
        if (ss >> k)
            cout << k;
        while (ss >> k) {
            if (k == ")")
                cout << k << endl;
            else
                cout << " " << k;
        }
        return true;
    }
    else if (!(save[index_].compare("REMOVE")) || !(save[index_].compare("remove"))) {
        remove();
        index_ = save_index - 1;
        return true;
    }
    else if (!(save[index_].compare("SUBST")) || !(save[index_].compare("subst"))) {
        subst();
        return true;
    }
    else if (!(save[index_].compare("PRINT")) || !(save[index_].compare("print"))) {
        print();
    }
    else if (token_save[index_] == IDENT) {
        string temp;
        if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
            LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

            while (1) {
                temp += checkPoint->data;
                temp += " ";

                if (checkPoint->includeNode == NULL)
                    break;

                checkPoint = checkPoint->includeNode;
            }
            stringstream ss(temp);
            string k;
            cout << "(";
            ss >> k;
            cout << k;
            while (ss >> k) {
                cout << " " << k;

            }
            cout << ")" << endl;


        }
        else return false;
    }


}

void print() {
    index_++;
    for (; index_ < save_index; index_++) {
        if (token_save[index_] == IDENT) {
            string temp;
            if ((findNode_horizon_data(save[index_]).compare("LISTDATA")) == 0) { // �����ϴ� ����� LIST
                LISP_list* checkPoint = findNode_horizon_addr(save[index_]);

                while (1) {
                    temp += checkPoint->data;
                    temp += " ";

                    if (checkPoint->includeNode == NULL)
                        break;

                    checkPoint = checkPoint->includeNode;
                }
                stringstream ss(temp);
                string k;
                cout << "(";
                ss >> k;
                cout << k;
                while (ss >> k) {
                    cout << " " << k;

                }
                cout << ")" << endl;


            }
            else if (findNode_horizon_data(save[index_]) == "NotExist") {
                cout << save[index_] << " doesn't exist" << endl;

            }
            else {
                cout << findNode_horizon_data(save[index_]) << endl;
            }
        }
        else if (save[index_] == "(") {

        }
        else if (save[index_] == ")") {
            break;
        }
        else {
            cout << save[index_] << endl;
        }
    }

}  