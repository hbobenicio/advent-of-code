import scala.collection.mutable.ListBuffer
import scala.io.Source
import scala.util.Using
import scala.util.matching.Regex

object Position {
    var horizontal: Int = 0
    var depth: Int = 0
    
    def forward(value: Int) =
        this.horizontal += value

    def up(value: Int) =
        this.depth -= value

    def down(value: Int) =
        this.depth += value
}

enum Direction:
    case Forward, Up, Down

class Command(val inputLine: String) {
    
}


@main
def main() =
    // var xs = ListBuffer[Command]()
    for (line <- Source.stdin.getLines) {
        // xs += line
    }
