#include <iostream>
#include <string>
#include <cctype>
#include <cstdint>
#include <sqlite3.h>

struct TokPair { const char *lex; const char *tag; };

bool alpha(char c){ return std::isalpha(static_cast<unsigned char>(c)); }
bool digit(char c){ return std::isdigit(static_cast<unsigned char>(c)); }



bool repeatedTag(const std::string &s,const std::string &piece){
    std::size_t n = piece.size();
    if(n==0||s.size()<n||s.size()%n) return false;
    for(std::size_t i=0;i<s.size();i+=n) if(s.compare(i,n,piece)!=0) return false;
    return true;
}


sqlite3* openDB(const char *path,int page){
    sqlite3 *db=nullptr;
    if(sqlite3_open(path,&db)!=SQLITE_OK){
        std::cerr<<"Cannot open DB\n";
        std::exit(1);
    }
    const char *pagesSQL = "CREATE TABLE IF NOT EXISTS pages(page_id INTEGER PRIMARY KEY, created_at DATETIME DEFAULT CURRENT_TIMESTAMP);";
    sqlite3_exec(db,pagesSQL,nullptr,nullptr,nullptr);
    std::string tbl = "CREATE TABLE IF NOT EXISTS page_"+std::to_string(page)+"(tok_id INTEGER PRIMARY KEY, type TEXT, lexeme TEXT);";
    sqlite3_exec(db,tbl.c_str(),nullptr,nullptr,nullptr);
    return db;
}

void makePageTable(sqlite3 *db,int page){
    std::string tbl = "CREATE TABLE IF NOT EXISTS page_"+std::to_string(page)+"(tok_id INTEGER PRIMARY KEY, type TEXT, lexeme TEXT);";
    sqlite3_exec(db,tbl.c_str(),nullptr,nullptr,nullptr);
}

void dbInsert(sqlite3 *db,int page,const std::string &kind,const std::string &lex){
    std::string sql = "INSERT INTO page_"+std::to_string(page)+"(type,lexeme) VALUES ('"+kind+"','"+lex+"');";
    sqlite3_exec(db,sql.c_str(),nullptr,nullptr,nullptr);
}

int main(int argc,char **argv){
    const char *dbPath = (argc>1? argv[1] : "tokens.db");

    TokPair kw[] = {
        {"\\section","SECTION"},{"\\subsection","SUBSECTION"},{"\\begin","BEGIN"},
        {"\\end","END"},{"\\frac","FRAC"},{"\\sum","SUM"},{"\\int","INTCMD"},
        {"\\newpage","NEWPAGE"},{"\\pagebreak","NEWPAGE"}
    };
    int KW = sizeof(kw)/sizeof(TokPair);

    const char* keywordLexemes[] = {"\\section","\\subsection","\\begin","\\end","\\frac","\\sum","\\int","\\newpage","\\pagebreak"};
    const char* keywordTokens[]  = {"SECTION","SUBSECTION","BEGIN","END","FRAC","SUM","INTCMD","NEWPAGE","NEWPAGE"};

    TokPair sy[] = {
        {"{","LBRACE"},{"}","RBRACE"},{"$","MATHMODE"},{"\\\\","NEWLINE"},
        {"^","CARET"},{"_","UNDERSCORE"},{"%","PERCENT"},{"&","AMPERSAND"},{"#","HASH"}
    };
    int SY = sizeof(sy)/sizeof(TokPair);

    const char* symbolLexemes[] = {"{","}","$","\\\\","^","_","%","&","#"};
    const char* symbolTokens[]  = {"LBRACE","RBRACE","MATHMODE","NEWLINE","CARET","UNDERSCORE","PERCENT","AMPERSAND","HASH"};

    const int32_t LIMIT_DIV10 = 214748364;  
    const int      LAST_DIG    = 7;        


    int page = 1;
    sqlite3 *db = openDB(dbPath,page);

    std::string token;
    while(std::getline(std::cin,token)){
        if(token==".ERROR"){ std::cerr<<"ERROR\n"; return 0; }

        if(token=="\\newpage"||token=="\\pagebreak"){ ++page; makePageTable(db,page); continue; }
        if(repeatedTag(token,".SPACE")||repeatedTag(token,".NEWLINE")) continue;
        if(token.size()>1&&token[0]=='%'&&token[1]=='%') continue; // LaTeX comment line

        bool emitted=false;
        for(int i=0;i<KW&&!emitted;++i){
            if(token==kw[i].lex){
                std::cout<<kw[i].tag<<' '<<token<<'\n';
                dbInsert(db,page,kw[i].tag,token);
                emitted=true;
            }
        }
        if(emitted) goto next_line;
        for(int i=0;i<SY&&!emitted;++i){
            if(token==sy[i].lex){
                std::cout<<sy[i].tag<<' '<<token<<'\n';
                dbInsert(db,page,sy[i].tag,token);
                emitted=true;
            }
        }
        if(emitted) goto next_line;

        if(alpha(token[0])||token[0]=='\\'){
            bool ok=true;
            for(char c:token) if(!alpha(c)&&!digit(c)&&c!='\\'){ ok=false;break; }
            if(ok){ std::cout<<"ID "<<token<<'\n'; dbInsert(db,page,"ID",token); goto next_line; }
        }

        if(digit(token[0])){
            for(char c:token) if(!digit(c)){ std::cerr<<"ERROR\n"; return 0; }
            if(token.size()>1&&token[0]=='0'){ std::cerr<<"ERROR\n"; return 0; }
            int32_t val=0;
            for(char c:token){
                int d=c-'0';
                if(val>LIMIT_DIV10||(val==LIMIT_DIV10&&d>LAST_DIG)){ std::cerr<<"ERROR\n"; return 0; }
                val=val*10+d;
            }
            std::cout<<"NUM "<<token<<'\n';
            dbInsert(db,page,"NUM",token);
            goto next_line;
        }

        std::cerr<<"ERROR\n"; return 0;

        next_line:; 

    }

    sqlite3_close(db);
    return 0;
} 