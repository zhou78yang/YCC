/*          block  comment test
                ************ //
    */          // /*  block useless      
        // line comment test
      //         */

public class TokenTest
{
    public static void main(String [] args)
    {
        
        int i1 = 0, i2 = 1234, i3 /* = 01534*/, i4 = 0372, i5 = 0xDadaCafe, i6 = 0x00FF00ff;
        long l1 = 01, l2 = 0777L, l3 = 0x1000000000L, l4 = 0XC0B0L;
        float f1 = 1e1f, f2 = .2f, f3 = 3.14, f4 = 0.22323e+23f;
        double d1 = 0x400A9F21P-3, d2 = 0x.APF, d3 = 0x0F0f01.C0B0P5;
        char c0 = '', c1 = 'a', c2 = '"', c3 = '\t', c4 = '\\', c5 = '\'', c6 = '\177', c7 = '\u03a9', c8 = '\uFfFf';
        String s0 = "", s1 = "'", s2 = "\"", s3 = "this is a string", s4 = "This is \101 string \u005F";
        boolean b1 = true;

        b1 = false;
        i1 += i2;
        i3 *= i4 + 5 / (10 * i5);
        String s = fun(i1, i2);

        s = null;
    }

    String fun(int a, int b)
    {
        /*
        if(a > b)
        {
            return a;
        }
        */
        while(b < a)
        {
            b += 2;
        }

        return "" + a + b;
    }
}
