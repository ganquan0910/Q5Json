function load3() {
    var txt = '';
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function(){
        console.log(xmlhttp.readyState);
        if(xmlhttp.status==200 && xmlhttp.readyState==4){
            txt=xmlhttp.responseText;
        }
    }
    xmlhttp.open("GET","https://raw.github.com/trev/bearcal/master/sample-data-large.json",true);
    xmlhttp.send();
    console.log(txt);
}

var txt = '';

function load() {

    listModel.clear();
    console.log("here");
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function()
    {
        if ( xhr.readyState == 4)
        {
            if ( xhr.status == 200)
            {
                //                var jsonObject = eval('(' + xhr.responseText + ')');
                var jsonObject = JSON.parse(xhr.responseText);
                txt = xhr.responseText;
//                console.log(txt);
                loaded(jsonObject);
            }
        }
    }
    xhr.open("GET","https://raw.github.com/trev/bearcal/master/sample-data-large.json",true);
    xhr.send();
}

function loadMore() {
    var jsonObject = JSON.parse(txt);
//    console.log("more")
    loaded(jsonObject);
}

function bench() {
    var oldTime = new Date();

    for (var i = 0; i < 10000; i++)
    {
        loadMore();
    }

    var newTime = new Date();

    console.log(newTime - oldTime);
}

function loaded(jsonObject)
{
    for ( var index in jsonObject.availability   )
    {
//        console.log("h");
        listModel.append({
                             "date" : jsonObject.availability[index].date,
                             "type" : jsonObject.availability[index].type,
                             "delimiter" : jsonObject.availability[index].delimiter});
    }
}
