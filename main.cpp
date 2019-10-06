#include <fstream>
#include <iostream>
using namespace std;

int atoi(string s){
	int index;
    for (int i = 0; i < s.length(); i++)
    {
        index = (int)s[i];
    }
    return index;
}

class treeNode{

	public:
		string chStr;
		int prob;
		treeNode *next;
		treeNode *left;
		treeNode *right;
		string code;

		treeNode(string str, int count){
			chStr = str;
			prob = count;
			next = NULL;
			left = NULL;
			right = NULL;
		}
		string printNode(treeNode *T){
			string node = T->chStr+" "+to_string(T->prob);
			if(T->next != NULL)
				node+=" "+T->next->chStr;
			if(T->left != NULL)
				node+=" "+T->left->chStr;			
			if(T->right != NULL)
				node+=" "+T->right->chStr;
	
			return node;
		}
};
class linkedList{
	public:
		treeNode *listHead;
		linkedList(treeNode *lhead){
			listHead = lhead;
		}
		void insertOneNode(treeNode *spot, treeNode *nnode){
			nnode->next = spot->next;
			spot->next = nnode;
		}
		treeNode *findSpot(treeNode *lhead, treeNode *nnode){
			treeNode *spot = lhead;// step 1: spot <-- LHead
			while(spot->next != NULL && spot->next->prob<nnode->prob){// 2: if(spot.next != null && spot.next.count < NewNode.count)
				spot = spot->next;// spot <-- spot.next
			}// 3: repeat step 2 until the condition fails
			return spot;// 4: return spot
		}
		void printList(treeNode *lhead, ofstream& outFile){

			string list = "listHead-->";
	
			while(lhead->next != NULL){
				list += string("(\"")+ string(lhead->chStr) + string("\", ")+to_string(lhead->prob) + string(", \"") + string(lhead->next->chStr)+ string("\")-->");
				lhead = lhead->next;
			}
			list += string("(\"") + string(lhead->chStr)+ string("\", ") +to_string(lhead->prob) + string(", NULL)-->NULL\n");
			outFile << list;
		}
};
class huffmanBinaryTree{
	public:
		treeNode *Root;
		treeNode *listHead;
		int charCount[256];
		string charCode[256];

		huffmanBinaryTree(){
			Root = NULL;
		}
		bool isLeaf(treeNode *T){
			bool isLeaf;
			if(T->left == NULL && T->right == NULL)
				return true;
			else
				return false;
		}
		void constructHuffmanLList(int charCount[], ofstream& out){
			listHead = new treeNode("dummy",0);
			linkedList ll(listHead);
			string chr;
			int prob, index = 0;
			while(index<256){
				if(charCount[index]>0){
					chr = char(index);
					prob = charCount[index];
					treeNode *nnode = new treeNode(chr, prob);
					treeNode *spot = ll.findSpot(listHead,nnode);
					ll.insertOneNode(spot, nnode);
				}
				index++;
			}
			ll.printList(listHead,out);
			out<<endl;
		}
		void constructHuffmanBinTree(treeNode *listHead, ofstream& outFile){
			linkedList ll(listHead);
			treeNode *nnode;
			while(listHead->next->next != NULL) {
				nnode = new treeNode(listHead->next->chStr + listHead->next->next->chStr,
											   listHead->next->prob + listHead->next->next->prob);
				nnode->left = listHead->next;
				nnode->right = listHead->next->next;
				treeNode *spot = ll.findSpot(listHead, nnode);
				ll.insertOneNode(spot, nnode);
				listHead->next = listHead->next->next->next;
				ll.printList(listHead, outFile);
			}
			Root = nnode;
		}

		void getCode(treeNode *T, string code, ofstream& outFile1){
			if(isLeaf(T)){
				T->code = code;
				int index = atoi(T->chStr);
				charCode[index] = code;
				outFile1<< T->chStr << " "<<T->code<<endl;
			} else{
				getCode(T->left, code+"0",outFile1);
				getCode(T->right, code+"1",outFile1);
			}
		}
		void computeCount(ifstream& inFile1,ofstream& outFile1){
			char charIn;
			int index;
			while(!inFile1.eof()){
				charIn = inFile1.get();
				index = charIn;//step 2: index <- cast charIn to integer
				charCount[index]++;
			    
			}
			printAry(outFile1, charCount);// step 5: printAry
		}
		void printAry(ofstream& outFile, int charCount[]){
			int index = 0;
			char symbol;
			while(index < 256){
				if(charCount[index]>0){
					symbol = (char)index;
					outFile<<symbol<<" "<<charCount[index]<<endl;
				}// step 4: repeat step 2 to step 3 while i < 256
				index++;// step 3: index ++
			}
			outFile<<endl;
		}
		void encode(ifstream& in, ofstream& out){
			char charIn;
			int index;
			string code;
			while(in.good()){
				charIn = in.get();
				index = (int)charIn;
				if(index <256 && index>=0){
					code = charCode[index];
					out<<code;
				}
			}
		}
		void decode(ifstream& in, ofstream& out,treeNode *T){
			char nextBit;
			if(isLeaf(T)){
				out<<T->chStr;
				decode(in,out,Root);
			}
			else {
				if(in.eof() && !isLeaf(T))
					out<<"the encode file is a corrupted file";
				else{
					nextBit = in.get();
					if(nextBit == '0')
						decode(in, out, T->left);
					if(nextBit == '1')
						decode(in, out, T->right);
				}	
			}
			
		}
};



int main(int argc, char const *argv[])
{	
	ifstream inFile1;
	inFile1.open(argv[1]);//An English text file to compute char-prob pairs.
	ifstream inFile2;
	inFile2.open(argv[2]);//An English text file for encoding.
	ofstream outFile1;
	outFile1.open(argv[3]);//A text file contains the Huffman <char	, code> pairs 
	ofstream outFile2;
	outFile2.open(argv[4]);// A text file contains the encoded result
	ofstream outFile3;
	outFile3.open(argv[5]);// A text file contains the decoded result.
	ofstream debug;
	debug.open(argv[6]);//  for all the  debugging outputs. 
	
	huffmanBinaryTree *hbt = new huffmanBinaryTree();
	
	hbt->computeCount(inFile1,debug);  
	
	hbt->constructHuffmanLList(hbt->charCount, debug);
	
	hbt->constructHuffmanBinTree(hbt->listHead, debug);

	debug.close();

	hbt->getCode(hbt->Root,"", outFile1);
	
	hbt->encode(inFile2, outFile2);
	
	outFile2.close();
	
	ifstream inFile3;
	string fileName;
	cout<<"enter file name: ";
	cin >> fileName;
	inFile3.open(fileName);
	hbt->decode(inFile3,outFile3,hbt->Root);
	
	inFile1.close();
	inFile2.close();
	inFile3.close();
	outFile1.close();
	outFile3.close();

	return 0;
}