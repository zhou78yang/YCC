public class CtrlFlow
{
    public static void main(String [] args)
    {
        int a = 123;
        int i;
        if(a < 100)
        {
            a += 100;
        }
        else
        {
            a -= 100;
        }
        for(i = 0; i < 100; i++)
        {
            a += i;
        }
        while(a > 0)
        {
            --a;
        }
        do
        {
            a++;
        }while(a < 123);
    }
}
