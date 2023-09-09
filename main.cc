#include <bits/stdc++.h>

#include "antlr4-runtime.h"
#include "Go4UnpackLexer.h"
#include "Go4UnpackParser.h"
#include "Go4UnpackBaseListener.h"

using namespace antlr4;
using namespace std;

namespace go4_unpack {
	std::string read_file(const char* fileName) {
		std::ifstream ifile(fileName);
		if(!ifile.is_open()) throw std::runtime_error("Can't open file !");
		std::stringstream b; b << ifile.rdbuf();

		return b.str();
	}
}

class TreeListener : public Go4UnpackBaseListener {
	std::ofstream out_file;

	std::stringstream for_block;
	int for_lb;
	int for_rb;
	string for_id;
	bool is_in_for = false;

	std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace) {
		size_t pos = 0;
		while((pos = subject.find(search, pos)) != std::string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

public:

   void enterClass_structure(Go4UnpackParser::Class_structureContext * ctx) override {
	   out_file = std::ofstream(ctx->class_declaration()->name()->IDENT()->getText() + "__.cc");
	   printf("Entered class_structure!\n");
   }
   void exitClass_structure(Go4UnpackParser::Class_structureContext * ctx) override {
	   out_file << "}\n";
	   out_file.close();
	   printf("Exited class_structure!\n");
   }

   void enterClass_declaration(Go4UnpackParser::Class_declarationContext * ctx) override { 
		out_file << "template<typename = void> \n";
		out_file << "structure " << ctx->name()->IDENT()->getText() << " {\n";
	   /* printf("Entered class_declaration!\n"); */
   }
   void exitClass_declaration(Go4UnpackParser::Class_declarationContext * ctx) override { 
	   /* printf("Exited class_declaration!\n"); */
   }
   void enterFor_loop(Go4UnpackParser::For_loopContext * ctx) override { 
		cout << ">> Entering for loop! .. \n";
		for_lb = stoi( ctx->for_left_bound()->DEC_NUMBER()->getText() );
		for_rb = stoi( ctx->for_right_bound()->DEC_NUMBER()->getText() );
		for_id = ctx->for_id()->IDENT()->getText();
		is_in_for = true;
   }
   void exitFor_loop(Go4UnpackParser::For_loopContext * ctx) override {
		string s_inside = for_block.str();
		
	   for(int id = for_lb; id < for_rb; ++id) {
		   cout << "Repeating: " << id << endl;
			out_file << "#define " << for_id << " " << id << endl;
			out_file << ReplaceString(s_inside, "@@", to_string(id));
			out_file << "#undef " << for_id << endl;
		}
		cout << "<< Exiting for loop! .. \n";

		for_block.clear();
		is_in_for = false;
   }
	 
   void enterClass_templates(Go4UnpackParser::Class_templatesContext * ctx) override { 
		cout << " ~~ Entered class templates" << endl;
   }
   void exitClass_templates(Go4UnpackParser::Class_templatesContext * ctx) override { 
		cout << " ~~ Exited class templates" << endl;
   }


   void enterBasic_field(Go4UnpackParser::Basic_fieldContext * ctx) override {
		cout << "Entered FIELD with name: " << ctx->name()->IDENT()->getText() << endl;
		if(is_in_for) 
		   for_block << ctx->type()->FIELD_TYPE()->getText() << " " << ctx->name()->IDENT()->getText() << "_@@;" << endl; 
		else 
			out_file << ctx->type()->FIELD_TYPE()->getText() << " " << ctx->name()->IDENT()->getText() << ";" << endl;
   }
};

int main(int argc, const char* argv[]) {

	auto s = go4_unpack::read_file("test.spec");

	ANTLRInputStream input(s);
    Go4UnpackLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    Go4UnpackParser parser(&tokens);    

	auto rule_names = lexer.getRuleNames();

	tokens.fill();
	/* for (auto token : tokens.getTokens()) { */
	/* 	std::cout << "Token: \"" << token->getText() << "\" .. Type: " << token->getType() << std::endl; */
	/* } */
   
	tree::ParseTree* tree = parser.start();
	TreeListener listener;

	tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    // Display the parse tree
    std::cout << parser.start()->toStringTree() << std::endl;
    return 0;
}
