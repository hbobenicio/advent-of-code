import scala.collection.mutable.ListBuffer
import scala.io.Source
import scala.util.Using

def solve(xs: List[Int]): Int = {
    var count = 0
    var prev = Int.MaxValue
    for (x <- xs) {
        if (prev < x) {
            count += 1
        }
        prev = x
    }
    count
}

@main
def main = {
    var xs = ListBuffer[Int]()
    for (line <- Source.stdin.getLines) {
        xs += line.toInt
    }
    println(solve(xs.toList))
}
