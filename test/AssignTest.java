public class AssignTest
{
    public static void main(String [] args)
    {
        int a, b, c, i;
        //a = 1 = b;
        //a = b = 1;
        a = 10;
        b = 5;
        //c = -1;
        if(a < 11)
        {
            b = 10;
            if(a > 0)
            {
                b = b - a;
                for(i = 1; i < a; i++)
                {
                    b++;
                    if(i == 5)
                    {
                        break;
                    }
                }
            }
        }
        
    }
}
