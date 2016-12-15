public class SwitchTest
{
    public static void main(String [] args)
    {
        int a = 1;
        switch(a)
        {
        case 1:
            a = 10;
            break;
        case '2':
            a *= 5;
            break;
        case 3:
            a /= 3;
            break;
        default:
            a = 0;
            break;
        }
    }
}
