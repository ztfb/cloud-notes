package com.cloudnotes.service.controller;

import com.cloudnotes.service.common.Result;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@CrossOrigin
public class TestController {
    @GetMapping("/test")
    public Result test(){return new Result("0","连接成功",null);}
}
