/**
 * @~english
 * @taomoduledescription{NodeJS, NodeJS interface}
 *
 * Execute JavaScript code inside a NodeJS subprocess. Call JavaScript from
 * Tao and vice versa.
 *
 * The NodeJS code can be read from a file with @ref nodejs_load.
 *
 * @code
import NodeJS

nodejs_load "file.js"
 * @endcode
 *
 * Or, you may pass the source as an argument to @ref nodejs.
 * @code
import NodeJS

nodejs "console.log('NodeJS version is: ' + process.version);"
 * @endcode
 *
 * Any data read from Node's @c stderr are sent to Tao's own @c stderr.
 * The same goes for @c stdout, except that lines starting with the characters
 * <tt>tao.</tt> are interpreted as XL code and evaluated by Tao.
 * @code
import NodeJS

nodejs <<
    console.log('This goes to Tao\'s stdout');
    console.error('This goes to Tao\'s stderr');
    console.log('tao.writeln "This is displayed by Tao\'s writeln function"');
>>
 * @endcode
 *
@verbatim
This goes to Tao's stdout
This goes to Tao's stderr
This is displayed by Tao's writeln function
@endverbatim
 *
 * Tao writes to the process @c stdin when @ref nodejs_writeln is called in
 * the document. This provides a simple way to have Tao invoke JavaScript
 * code when it needs to, as the following example demonstrates.
 *
 * Several NodeJS processes may be run concurrently: just give a name to each
 * process when you call @ref nodejs or @ref nodejs_load.
 *
 * @code
import NodeJS

nodejs "#1", <<
    console.log('This is Node #1');
    console.log('tao.send_to_node_2 "Hi from Node #1"');
>>

nodejs "#2", <<
    process.stdin.resume();
    process.stdin.on('data', function(chunk) {
        var buffer = '';
        buffer += chunk;
        buffer = buffer.replace(/[\n\r]/g, '').trim();
        console.log('Tao says: ' + buffer);
    });
>>

send_to_node_2 Message:text ->
  nodejs_writeln "#2", Message
 * @endcode
 *
 * The above code will print:
 *
@verbatim
This is Node #1
Tao says: Hi from Node #1
 @endverbatim
 *
 * @endtaomoduledescription{NodeJS}
 *
 * @~french
 * @taomoduledescription{NodeJS, Interface NodeJS}
 *
 * Permet d'exécuter du code JavaScript dans un processus NodeJS, d'appeler du
 * JavaScript depuis Tao et réciproquement.
 *
 * Le code NodeJS peut être lu depuis un fichier grâce à @ref nodejs_load.
 *
 * @code
import NodeJS

nodejs_load "file.js"
 * @endcode
 *
 * Ou bien vous pouvez passer directement le code source à @ref nodejs :
 * @code
import NodeJS

nodejs "console.log('La version de NodeJS est : ' + process.version);"
 * @endcode
 *
 * Tout ce qui est lu depuis le canal @c stderr du processus Node est envoyé
 * sur le canal @c stderr de Tao.
 * De même pour @c stdout, sauf que les lignes qui commencent
 * par les caractères <tt>tao.</tt> sont supposées être du code XL et sont
 * évaluées par Tao.
 * @code
import NodeJS

nodejs <<
    console.log('Ce message va vers stdout de Tao');
    console.error('Ce message va vers stderr de Tao');
    console.log('tao.writeln "Ceci est affiché par la primitive writeln de Tao"');
>>
 * @endcode
 *
@verbatim
Ce message va vers stderr de Tao
Ce message va vers stdout de Tao
Ceci est affiché par la primitive writeln de Tao
@endverbatim
 *
 * Tao écrit sur le canal @c stdin lorsque le document appelle
 * @ref nodejs_writeln, ce qui fournit un moyen simple d'appeler du code
 * JavaScript depuis Tao comme le montre l'exemple suivant.
 *
 * Plusieurs processus NodeJS peuvent s'exécuter simultanément. Il suffit pour
 * cela de donner un nom différent à chacun lors de l'appel à @ref nodejs ou
 * @ref nodejs_load.
 *
 * @code
import NodeJS

nodejs "1", <<
    console.log('Ici Node 1');
    console.log('tao.send_to_node_2 "Salut depuis Node 1"');
>>

nodejs "2", <<
    process.stdin.resume();
    process.stdin.on('data', function(chunk) {
        var buffer = '';
        buffer += chunk;
        buffer = buffer.replace(/[\n\r]/g, '').trim();
        console.log('Tao dit: ' + buffer);
    });
>>

send_to_node_2 Message:text ->
  nodejs_writeln "2", Message
 * @endcode
 *
 * Le code ci-dessus affiche:
 *
@verbatim
Ici Node 1
Tao dit: Salut depuis Node 1
 @endverbatim
 *
 * @endtaomoduledescription{NodeJS}
 *
 * @~
 * @{
 */

/**
 * @~english
 * Starts a NodeJS subprocess to execute @p src.
 * The process is automatically restarted whenever @p src changes.
 * This primitive is equivalent to <tt>nodejs "default", src</tt>.
 *
 * @~french
 * Lance un processus fils NodeJS et exécute @p src.
 * Le processus est redémarré automatiquement lorsque @p src change.
 * Cette primitive est équivalente à <tt>nodejs "default", src</tt>.
 * @~
 * @code
import NodeJS

nodejs <<
  setInterval(function() {
    console.log('Hello');
  }, 2000);
>>
 * @endcode
 */

boolean nodejs(src:text);

/**
 * @~english
 * Starts a NodeJS subprocess called @p name to execute @p src.
 * @~french
 * Lance un processus fils NodeJS nommé @p name, et exécute @p src.
 * @~
 * @code
import NodeJS

nodejs "A", <<
    var count = 1;
    setInterval(function() {
        console.log(count++);
    }, 1000);
>>

nodejs "B", <<
    setTimeout(function() {
        console.log('tao.exit 0');
    }, 5500);
>>
 * @endcode
 */

boolean nodejs(name: text, src:text);

/**
 * @~english
 * Starts a NodeJS subprocess to execute file @p path.
 * The process is automatically restarted whenever the file is modified.
 * This primitive is equivalent to <tt>nodejs_load "default", path</tt>.
 *
 * @~french
 * Lance un processus fils NodeJS et exécute le fichier @p path.
 * Le processus est redémarré automatiquement lorsque le fichier
 * est modifié.
 * Cette primitive est équivalente à <tt>nodejs_load "default", path</tt>.
 */

boolean nodejs_load(path:text);

/**
 * @~english
 * Starts a NodeJS subprocess called @p name to execute file @p path.
 * @~french
 * Lance un processus fils NodeJS nommé @p name, et exécute le fichier @p path.
 */

boolean nodejs_load(name: text, path:text);

/**
 * @~english
 * Sends @p str followed by an end-of-line to the default NodeJS suprocess.
 * If the NodeJS process is running, this primitive returns @c true, otherwise
 * it returns @c false.
 * This primitive is equivalent to <tt>nodejs_writeln "default", str</tt>.
 *
 * @~french
 * Envoie @p str et une fin de ligne au processus NodeJS par défaut.
 * Si le processus est en cours d'exécution, cette primitive retourne @c true,
 * sinon @c false.
 * Cette primitive est équivalente à <tt>nodejs_writeln "default", str</tt>.
 * @~
 */
boolean nodejs_writeln(str:text);

/**
 * @~english
 * Sends @p str followed by an end-of-line to the NodeJS suprocess called
 * @p name.
 * If the NodeJS process is running, this primitive returns @c true, otherwise
 * it returns @c false.
 *
 * @~french
 * Envoie @p str et une fin de ligne au processus NodeJS nommé @p name.
 * Si le processus est en cours d'exécution, cette primitive retourne @c true,
 * sinon @c false.
 * @~
 */
boolean nodejs_writeln(name:text, str:text);

/**
 * @}
 */
