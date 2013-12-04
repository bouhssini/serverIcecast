#include "General.h"

short cts(unsigned char * ch)
{
        short i;
        unsigned char ptr[2];
        ptr[0]=ch[0];
        ptr[1]=ch[1];
        memcpy(&i,&ptr,sizeof(short));
        return i;
}
string convertInt(int value) {
  std::ostringstream o;
  if (!(o << value))
    return "";
  return o.str();
}

string itostr(int value)
{
  std::ostringstream o;
  if (!(o << value))
    return "";
  return o.str();
}
int strtoi(string s)
{
    return atoi(s.c_str());
}

string stc(short i)
{
    string s;
        char ptr[2];
        memcpy(&ptr,&i,sizeof(short));
        s.append(ptr,2);
        return s;
}
string itc(int i)
{
        string s;
        char ptr[4];
        memcpy(&ptr,&i,sizeof(int));
        s.append(ptr,4);
        return s;
}
int cti(unsigned char * ch)
{
        int i;
        unsigned char ptr[4];
        ptr[0]=ch[0];
        ptr[1]=ch[1];
        ptr[2]=ch[2];
        ptr[3]=ch[3];
        memcpy(&i,&ptr,sizeof(int));
        return i;
}

string Replace(string txt,string str,string chstr)
{
    int pos=0,ln=0;
    if (str == chstr)
        return txt;
    if (str == "")
        return txt;

    while((pos = txt.find(str,ln))!=-1)
    {
        txt.replace(pos,str.length(),chstr);
        ln=pos+chstr.length()-str.length()+1;
    }
    return txt;
}
string Trim(string txt)
{
    char sp=' ';
    unsigned int i=0;
    string tx2;
    for (i=0;i<txt.length();i++)
          if (txt.data()[i] != sp)  break;

    tx2 = txt.substr(i,txt.length());

    for (i=1;i<tx2.length();i++)
          if (tx2.data()[tx2.length()-i] != sp)  break;

    tx2 = tx2.substr(0,tx2.length()-i+1);
    return tx2;
}
string String(int Ln,char c)
{
    char s[Ln];
    string s2;
    for(int i=0;i<Ln;i++)
        s[i] = c;
    s2.append(s,Ln);
    return s2;
}
string ICONV(string txt,const char fromcode[],const char tocode[])
{
    iconv_t      cd;
    size_t       in_size = txt.length();
    char         *inptr = (char*)txt.c_str();
    char         out[in_size*3];
    size_t       out_size = sizeof(out);
    char         *outptr = out;

    string s;
    s.clear();

    if ((iconv_t)(-1) == (cd = iconv_open(tocode, fromcode))) {
       goto fin;
    }
    if ((size_t)(-1) == iconv(cd, &inptr, &in_size, &outptr, &out_size)) {
       goto fin;
    }
    *outptr = '\0';
    if (-1 == iconv_close(cd)) {
       goto fin;
    }
    s.append(out,strlen(out));
    fin:
    return s;
}

vector<string> Split(string txt,string str)
{
    vector<string> l;
    int pos=0,ln=0,ps=0,i=0;
    string tx;
    if (str == "")
    {
        l.push_back(txt);
        return l;
    }

    while((pos = txt.find(str,ln))!=-1)
    {
        tx = txt.substr(ps,pos-ps);
        l.push_back(tx);
        //ln=pos+str.length()+1;
        ps=pos+str.length();
        ln = ps;
        i++;
    }

    if (i == 0)
    {
        l.push_back(txt);
    }else{
        tx = txt.substr(ps,txt.length()-ps);
        l.push_back(tx);
    }
    return l;
}

string UCASE(string str)
{
    int leng=str.length();
    for(int i=0; i<leng; i++)
        if (97<=str[i]&&str[i]<=122)//a-z
            str[i]-=32;
    return str;
}
string LCASE(string str)
{
    int leng=str.length();
    for(int i=0; i<leng; i++)
        if (65<=str[i]&&str[i]<=90)//A-Z
            str[i]+=32;
    return str;
}
