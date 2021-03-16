#include <mxml.h>
#include <stdio.h>

void printProperXML(FILE* stream, mxml_node_t* parent, int level) {
	if (parent && mxmlGetElement(parent)) {
		for(int i = 0; i < level; i++) {
			fprintf(stream, "\t"); }
		fprintf(stream, "<%s", mxmlGetElement(parent));
		int attr_count = mxmlElementGetAttrCount(parent);
		if (!attr_count) { fprintf(stream, ">\n"); }
		else {
			for(int i = 0; i < attr_count; i++) {
				const char* attr_name;
				const char* attr_val = mxmlElementGetAttrByIndex(parent, i, &attr_name);
				fprintf(stream, "\n");
				for(int i = 0; i < level + 1; i++) {
					fprintf(stream, "\t"); }
				fprintf(stream, "%s=\"%s\"", attr_name, attr_val);
			}
			fprintf(stream, " ");
		}
	}
	mxml_node_t* kid = mxmlGetFirstChild(parent);
	if (kid) {
		fprintf(stream, ">\n");
		kid = mxmlGetNextSibling(kid);						//a parent's first child is an imaginary node
		while(kid && kid != mxmlGetLastChild(parent)) {
			printProperXML(stream, kid, level + 1);
			kid = mxmlGetNextSibling(mxmlGetNextSibling(kid));		//for some reason, an element's direct next sibling is an imaginary node
		}
		for(int i = 0; i < level; i++) {
			fprintf(stream, "\t");
		}
		fprintf(stream, "</%s>\n", mxmlGetElement(parent));
	}
	else if (parent) { fprintf(stream, "/>\n"); }
}

void printXMLBase(FILE* stream, mxml_node_t* parent) {
	fprintf(stream, "<%s>\n", mxmlGetElement(parent));
	printProperXML(stream, mxmlGetNextSibling(mxmlGetFirstChild(parent)), 0);
}

int main(int argc, char** argv) {
	FILE* file;
	mxml_node_t* tree;

	file = fopen("data/parsethis.xml", "r");
	tree = mxmlLoadFile(NULL, file, MXML_TEXT_CALLBACK);
	fclose(file);


	file = fopen("data/comparethat.xml", "w");
	printXMLBase(file, tree); //tree contains the "?xml..." tag at the top, so you go one down from that
	fclose(file);

	mxmlDelete(tree);

	return 0;
}
