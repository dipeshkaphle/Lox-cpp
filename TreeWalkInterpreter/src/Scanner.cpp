#include "Scanner.hpp"
using tok = TokenType;

const unordered_map<string, TokenType> Scanner::keywords = {
    {"and", tok::AND},     {"or", tok::OR},       {"class", tok::CLASS},
    {"else", tok::ELSE},   {"false", tok::FALSE}, {"true", tok::TRUE},
    {"for", tok::FOR},     {"fn", tok::FN},       {"if", tok::IF},
    {"nil", tok::NIL},     {"print", tok::PRINT}, {"return", tok::RETURN},
    {"super", tok::SUPER}, {"this", tok::THIS},   {"let", tok::LET},
    {"while", tok::WHILE}};

char Scanner::peek() {
  if (is_at_end())
    return '\0';
  return source[this->cur];
}

char Scanner::peek_next() {
  if (this->cur + 1 >= source.size())
    return '\0';
  return source[this->cur + 1];
}

bool Scanner::match(char expected) {
  if (is_at_end())
    return false;
  if (source[cur] != expected)
    return false;
  this->cur++;
  return true;
}

void Scanner::add_token(TokenType type, std::any literal) {
  string text = source.substr(start, cur - start);
  tokens.emplace_back(Token(type, text, literal, cur_line));
}

void Scanner::get_string() {
  while (!is_at_end() && peek() != '"') {
    if (peek() == '\n')
      cur_line++;
    advance();
  }
  if (is_at_end()) {
    Lox::error(cur_line, "Unterminated string");
    return;
  }

  /*
   * was a valid string , so we advance further
   */
  advance();

  /*
   * source: "abc"x
   * start : 0
   * cur : 5
   * we want 1 to  3
   * so we do substr(start+1, (cur-2)-(start+1) + 1)
   */
  string str =
      source.substr(start + 1, (this->cur - 2) - (this->start + 1) + 1);
  add_token(TokenType::STRING, str);
}

void Scanner::get_number() {
  /*
   * consume all the digits
   */
  while (isdigit(peek()))
    advance();

  /*
   * means we've encountered a double
   */
  if (peek() == '.' && isdigit(peek_next())) {
    advance();
    while (isdigit(peek()))
      advance();
  }

  /*
   * source: 123.56+123
   * start : 0
   * cur: 6
   * wanted : 123.56
   * source.substr(0,cur-1 - start +1 )
   */
  string number = source.substr(start, (cur - 1) - start + 1);
  add_token(TokenType::NUMBER, std::stod(number));
}

void Scanner::get_identifier() {
  while (isalpha(peek()) || isdigit(peek()) || peek() == '_') {
    advance();
  }

  /*
   * abc = 2;
   * start =0
   * cur= 3
   *
   */
  string ident = source.substr(start, this->cur - this->cur);
  TokenType type = tok::IDENTIFIER;
  if (keywords.contains(ident)) {
    type = keywords.at(ident);
  }
  add_token(type, ident);
}

void Scanner::multiline_comment() {
  vector<string> st;
  st.push_back("/*");
  while (!st.empty()) {
    if (is_at_end()) {
      Lox::error(cur_line, "Unclosed multi line comment");
      return;
    }
    if (peek() == '/') {
      advance();
      if (!is_at_end() && peek() == '*') {
        // another multi line comment starts
        advance();
        st.push_back("/*");

      } else if (!is_at_end()) {
        advance();
      }

    } else if (peek() == '*') {

      advance();
      if (!is_at_end() && peek() == '/') {
        // a multi line comment closes
        advance();
        st.pop_back();
      }
    } else if (peek() == '\n') {
      advance();
      this->cur_line++;
    } else {
      advance();
    }
  }
}

vector<Token> Scanner::scan_tokens() {
  while (!is_at_end()) {
    start = cur;
    scan_token();
  }
  tokens.emplace_back(Token(TokenType::ENDOFFILE, "", "", cur_line));
  return tokens;
}

void Scanner::scan_token() {
  char c = this->advance();
  using tok = TokenType;
  switch (c) {
  case ' ':
  case '\r':
  case '\t':
    // Ignore whitespace.
    break;

  case '\n':
    this->cur_line++;
    break;
  case '(':
    add_token(tok::LEFT_PAREN);
    break;
  case ')':
    add_token(tok::RIGHT_PAREN);
    break;
  case '{':
    add_token(tok::LEFT_BRACE);
    break;
  case '}':
    add_token(tok::RIGHT_BRACE);
    break;
  case ',':
    add_token(tok::COMMA);
    break;
  case '.':
    add_token(tok::DOT);
    break;
  case '-':
    add_token(tok::MINUS);
    break;
  case '+':
    add_token(tok::PLUS);
    break;
  case ';':
    add_token(tok::SEMICOLON);
    break;
  case '*':
    if (!is_at_end() && peek() == '/') {
      advance(), Lox::error(cur_line, "Invalid token */");
      break;
    }
    add_token(tok::STAR);
    break;
  case '!':
    add_token(match('=') ? tok::BANG_EQUAL : tok::BANG);
    break;
  case '=':
    add_token(match('=') ? tok::EQUAL_EQUAL : tok::EQUAL);
    break;
  case '<':
    add_token(match('=') ? tok::LESS_EQUAL : tok::LESS);
    break;
  case '>':
    add_token(match('=') ? tok::GREATER_EQUAL : tok::GREATER);
    break;
  case '/': // multi line nested comments
    if (match('/')) {
      // A comment goes until the end of the line.
      while (!is_at_end() && peek() != '\n')
        advance();
    } else if (match('*')) {
      multiline_comment();
    } else {
      add_token(tok::SLASH);
    }
    break;
  case '"':
    get_string();
    break;

  default:
    if (isdigit(c)) {
      get_number();
    } else if (isalpha(c) || c == '_') {
      get_identifier();
    } else
      Lox::error(cur_line, "Unexpected character.");
    break;
  }
}
