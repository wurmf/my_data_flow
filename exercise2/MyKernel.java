public class MyKernel extends Kernel{

    public MyKernel(KernelParameters parameters){
        DFEVar p = io.input("p",dfeFloat(8,24));
        DFEVar s = io.scalarInput("c",dfeFloat(8,24));

        DFEVar q = p > k ? p + c : p * c;

        io.output("q",q,dfeFloat(8,24));
    }
}
        
